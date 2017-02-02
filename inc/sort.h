#ifndef __SORT_H__
#define __SORT_H__

#include <vector>

#include "utils.h"

using std::vector;

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

}


#endif /* end of include guard: __SORT_H__ */
