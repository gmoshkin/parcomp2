#ifndef __SORT_H__
#define __SORT_H__

#include <vector>
#include <algorithm>

#include "utils.h"

using std::vector;
using std::swap;

namespace Sort {

template <typename T>
void mergeSortSegment(vector<T> &v, vector<T> &tmp, size_t start, size_t end)
{
    size_t segmentLen = end - start;
    if (segmentLen < 2) {
        return;
    }
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

template <typename T>
void mergeSort(vector<T> &v)
{
    vector<T> tmp;
    tmp.reserve(v.size());
    mergeSortSegment(v, tmp, 0, v.size());
}

inline size_t leftChild(size_t i)
{
    return (i + 1) * 2 - 1;
}

inline size_t rightChild(size_t i)
{
    return (i + 1) * 2;
}

inline size_t parent(size_t i)
{
    return (i + 1) / 2 - 1;
}

template <typename T>
void fixRoot(vector<T> &v, size_t root, size_t end)
{
    size_t oldRoot = root;
    size_t left = leftChild(oldRoot);
    size_t right = rightChild(oldRoot);
    size_t newRoot = oldRoot;
    for (; left < end;
         oldRoot = newRoot,
         left = leftChild(newRoot),
         right = rightChild(newRoot)) {
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

template <typename T>
inline void formHeap(vector<T> &v)
{
    for (long root = parent(v.size() - 1); root >= 0; root--) {
        fixRoot(v, root, v.size());
    }
}

template <typename T>
void heapSort(vector<T> &v)
{
    formHeap(v);

    for (size_t end = v.size() - 1; end > 0; end--) {
        swap(v[0], v[end]);
        fixRoot(v, 0, end);
    }
}

}


#endif /* end of include guard: __SORT_H__ */
