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

    MPIWrapper &mpi;
    container_t ourData;
    container_t theirData;

public:
    MPISorter(MPIWrapper &mpi, vector<T> &data) : mpi(mpi), ourData(data) {}

    const container_t &getOurData() const
    {
        return this->ourData;
    }

    double sortAll()
    {
        this->mpi.startParallelSortTimer();
        size_t procCount = this->mpi.getProcCount();
        this->getSchedule(procCount, 0);
        return this->mpi.finishParallelSortTimer();
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
        this->mpi.startSortTimer();
        Sort::heapSort(this->ourData);
        return this->mpi.finishSortTimer();
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
    }
};

}

#endif /* end of include guard: __MPISCHEDULER_H__ */
