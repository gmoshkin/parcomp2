#ifndef __MPIWRAPPER_H__
#define __MPIWRAPPER_H__

#include <mpi.h>

class MPIWrapper
{
private:
    int argc;
    char **argv;
    int rank, procCount;

public:
    MPIWrapper(int argc, char **argv)
    {
        this->argc = argc;
        this->argv = argv;
        MPI_Init(&this->argc, &this->argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
        MPI_Comm_size(MPI_COMM_WORLD, &this->procCount);
    }
    ~MPIWrapper()
    {
        MPI_Finalize();
    }

    int getRank()
    {
        return this->rank;
    }
    int getProcCount()
    {
        return this->procCount;
    }
};

#endif /* end of include guard: __MPIWRAPPER_H__ */
