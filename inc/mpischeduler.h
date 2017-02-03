#ifndef __MPISCHEDULER_H__
#define __MPISCHEDULER_H__

#include "scheduler.h"

namespace Batcher {

class MPIScheduler : public Scheduler
{
private:
    MPIWrapper &mpi;

public:
    MPIScheduler(MPIWrapper &mpi) : mpi(mpi) {}

    void sortPair(node_t n1, node_t n2)
    {
        mpi.sortPair(n1, n2);
    }
}

}

#endif /* end of include guard: __MPISCHEDULER_H__ */
