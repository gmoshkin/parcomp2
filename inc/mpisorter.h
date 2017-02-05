#ifndef __MPISCHEDULER_H__
#define __MPISCHEDULER_H__

#include <algorithm>
#include <vector>

#include "scheduler.h"
#include "mpiwrapper.h"
#include "sort.h"

using std::vector;
using std::swap;

namespace Batcher {

template <typename T>
class MPISorter : public Scheduler
{
private:
    typedef vector<T> container_t;
    typedef typename container_t::iterator iterator_t;
    typedef typename container_t::reverse_iterator riterator_t;

    size_t threshold;
    MPIWrapper &mpi;
    container_t ourData;
    container_t theirData;

public:
    MPISorter(MPIWrapper &mpi, vector<T> &data, size_t threshold)
        : threshold(threshold), mpi(mpi), ourData(data)
    {
#ifdef COMBINED_SORT
        MPILOG(mpi, "Threshold is " << this->threshold << std::endl);
#else
        MPILOG(mpi, "Mergesort only" << std::endl);
#endif
        MPILOG(mpi, "Data size " << this->ourData.size() << std::endl);
    }

    const container_t &getOurData() const
    {
        return this->ourData;
    }

    double sortAll()
    {
        this->mpi.startTimer("parallel sort");
        size_t procCount = this->mpi.getProcCount();
        this->getSchedule(procCount, 0);
        return this->mpi.finishTimer("parallel sort", true);
    }

    void sortPair(node_t n1, node_t n2)
    {
        if (this->mpi.getRank() == n1) {
            mpi.exchangeData(n2, this->ourData, this->theirData);
            mergeBottom();
        } else if (this->mpi.getRank() == n2) {
            mpi.exchangeData(n1, this->ourData, this->theirData);
            mergeTop();
        }
    }

    double sortOur()
    {
        this->mpi.startTimer("sort");
#ifdef COMBINED_SORT
        if (this->ourData.size() < this->threshold) {
            this->heapSort(this->ourData);
        } else {
            this->mergeSort(this->ourData);
        }
#else
        this->mergeSort(this->ourData);
#endif
        return this->mpi.finishTimer("sort", true);
    }

    void mergeBottom()
    {
        container_t bottom;
        bottom.reserve(this->ourData.size());
        iterator_t ourIt = this->ourData.begin();
        iterator_t theirIt = this->theirData.begin();
        for (size_t i = 0; i < this->ourData.size(); i++) {
            if (*ourIt < *theirIt) {
                bottom.push_back(*ourIt);
                ourIt++;
            } else {
                bottom.push_back(*theirIt);
                theirIt++;
            }
        }
        this->ourData = bottom;
    }

    void mergeTop()
    {
        container_t top;
        top.reserve(this->ourData.size());
        riterator_t ourIt = this->ourData.rbegin();
        riterator_t theirIt = this->theirData.rbegin();
        for (size_t i = 0; i < this->ourData.size(); i++) {
            if (*ourIt < *theirIt) {
                top.push_back(*theirIt);
                theirIt++;
            } else {
                top.push_back(*ourIt);
                ourIt++;
            }
        }
        this->ourData.assign(top.rbegin(), top.rend());
    }

    void extendSize()
    {
        size_t ourSize = this->ourData.size();
        size_t difference = this->mpi.getMaxSize(ourSize) - ourSize;
        if (difference <= 0) {
            return;
        }
        while (difference) {
            this->ourData.push_back(T::makeDummy());
            difference--;
        }
        MPILOG(mpi, "Max size per process: " << mpi.getMaxSize() << std::endl);
    }

    void logSlice(container_t &v, size_t start, size_t end)
    {
        MPILOG(mpi, "{ ");
        for (size_t i = start; i < end; i++) {
            MPILOG(mpi, v[i] << " ");
        }
        MPILOG(mpi, "}");
    }

    void mergeSortSegment(container_t &v, container_t &tmp, size_t start, size_t end)
    {
        size_t segmentLen = end - start;
        if (segmentLen < 2) {
            return;
        }
#ifdef COMBINED_SORT
        if (segmentLen < this->threshold) {
            this->heapSort(v, start, end);
            return;
        }
#endif
        size_t middle = (start + end) / 2;
        mergeSortSegment(v, tmp, start, middle);
        mergeSortSegment(v, tmp, middle, end);

        size_t left = start;
        size_t right = middle;
        for (size_t i = start; i < end; i++) {
            if (left < middle && (right >= end || v[left] < v[right])) {
                tmp[i] = v[left++];
            } else {
                tmp[i] = v[right++];
            }
        }

        for (size_t i = start; i < end; i++) {
            v[i] = tmp[i];
        }
    }

    void mergeSort(container_t &v)
    {
        container_t tmp;
        tmp.reserve(v.size());
        mergeSortSegment(v, tmp, 0, v.size());
    }

    inline size_t leftChild(size_t i, size_t i0 = 0)
    {
        return (i - i0 + 1) * 2 - 1 + i0;
    }

    inline size_t rightChild(size_t i, size_t i0 = 0)
    {
        return (i - i0 + 1) * 2 + i0;
    }

    inline size_t parent(size_t i, size_t i0 = 0)
    {
        return (i - i0 + 1) / 2 - 1 + i0;
    }

    void fixRoot(container_t &v, size_t root, size_t end, size_t i0 = 0)
    {
        size_t oldRoot = root;
        size_t left = leftChild(oldRoot, i0);
        size_t right = rightChild(oldRoot, i0);
        size_t newRoot = oldRoot;
        for (; left < end;
            oldRoot = newRoot,
            left = leftChild(newRoot, i0),
            right = rightChild(newRoot, i0)) {
            if (v[newRoot] < v[left]) {
                newRoot = left;
            }
            if ((right < end) && (v[newRoot] < v[right])) {
                newRoot = right;
            }
            if (newRoot == oldRoot) {
                break;
            }
            swap(v[oldRoot], v[newRoot]);
        }
    }

    inline void formHeap(container_t &v, int start = -1, int end = -1)
    {
#ifdef COMBINED_SORT
        if (start >= 0 && end >= 0) {
            for (long root = parent(end - 1, start); root >= start; root--) {
                fixRoot(v, root, end, start);
            }
        }
        else {
            for (long root = parent(v.size() - 1, start); root >= 0; root--) {
                fixRoot(v, root, v.size());
            }
        }
#else
        for (long root = parent(v.size() - 1, start); root >= 0; root--) {
            fixRoot(v, root, v.size());
        }
#endif
    }

    void heapSort(container_t &v, int start = -1, int end = -1)
    {
#ifdef COMBINED_SORT
        if (start >= 0 && end >= 0) {
            formHeap(v, start, end);

            for (end--; end > start; end--) {
                swap(v[start], v[end]);
                fixRoot(v, start, end, start);
            }
        } else {
            formHeap(v);

            for (size_t end = v.size() - 1; end > 0; end--) {
                swap(v[0], v[end]);
                fixRoot(v, 0, end);
            }
        }
#else
        formHeap(v);

        for (size_t end = v.size() - 1; end > 0; end--) {
            swap(v[0], v[end]);
            fixRoot(v, 0, end);
        }
#endif
    }
};

}

#endif /* end of include guard: __MPISCHEDULER_H__ */
