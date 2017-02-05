#include <algorithm>
#include <iostream>

#include "scheduler.h"
#include "utils.h"
#include "sort.h"

using std::cout;
using std::endl;
using std::swap;
using std::vector;

void swapPair(numbers_t &l1, numbers_t &l2, unsigned i1, unsigned i2)
{
    size_t size = l1.size();
    if (i1 < size) {
        if (i2 < size) {
            swap(l1[i1], l1[i2]);
        } else {
            swap(l1[i1], l2[i2 - size]);
        }
    } else {
        if (i2 < size) {
            swap(l2[i1 - size], l1[i2]);
        } else {
            swap(l2[i1 - size], l2[i2 - size]);
        }
    }
}

int Sort::sort(numbers_t &l1, numbers_t &l2)
{
    Batcher::Scheduler sched;

    cout << l1 << " " << l2 << endl;

    sched.getSchedule(l1.size(), l2.size());

    int i1, i2;
    while(sched.getPair(i1, i2)) {
        swapPair(l1, l2, i1, i2);
    }
    cout << l1 << " " << l2 << endl;

    return 0;
}
