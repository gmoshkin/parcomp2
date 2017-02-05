#ifndef __MPIWRAPPER_H__
#define __MPIWRAPPER_H__

#include <mpi.h>
#include <vector>
#include <string>
#include <sstream>
#include <ostream>

#include "mpitimer.h"
#include "logger.h"
#include "utils.h"

#define MPILOG(logger, msg) \
    do {\
        std::stringstream ss;\
        ss << msg;\
        logger.log(ss.str());\
    } while(0);

using std::vector;
using std::endl;

class MPIWrapper
{
private:
    int argc;
    char **argv;
    size_t maxSize;
    int rank, procCount;
    MPITimer *sortTimer;
    MPITimer *parallelSortTimer;
    MPITimer *dataExchangeTimer;
    Logger *logger;

public:
    MPIWrapper(int argc, char **argv) : argc(argc), argv(argv), maxSize(0),
    rank(-1), procCount(0), sortTimer(NULL), parallelSortTimer(NULL)
    {
        MPI_Init(&this->argc, &this->argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
        MPI_Comm_size(MPI_COMM_WORLD, &this->procCount);

        std::stringstream name;
        name << "log_";
        name << this->getRank();
        this->logger = new Logger(name.str());
    }
    ~MPIWrapper()
    {
        MPI_Finalize();
        if (this->sortTimer != NULL) {
            delete this->sortTimer;
        }
        if (this->logger != NULL) {
            delete this->logger;
        }
    }

    void log(const std::string &msg)
    {
        if (this->logger != NULL) {
            this->logger->log(msg);
        }
    }

    void logTimers()
    {
        std::stringstream ss;
        ss << "linear sort: { " << this->sortTimer << " }" << endl;
        ss << "parallel sort: { " << this->parallelSortTimer << " }" << endl;
        ss << "data exchange: { " << this->dataExchangeTimer << " }" << endl;
        this->log(ss.str());
    }

    template <typename T>
    double exchangeData(int theirRank, const vector<T> &our, vector<T> &their)
    {
        this->startDataExchangeTimer();
        MPI_Datatype type;
        T::getMPIDatatype(type);
        size_t count = this->getMaxSize();
        const T *ourBuf = vector2array(our);
        MPI_Request sendReq;
        MPI_Isend(ourBuf, count, type, theirRank, 0, MPI_COMM_WORLD, &sendReq);

        T *theirBuf = new T[count];
        MPI_Request recvReq;
        MPI_Irecv(theirBuf, count, type, theirRank, 0, MPI_COMM_WORLD, &recvReq);

        MPI_Status status;
        MPI_Wait(&sendReq, &status);
        MPI_Wait(&recvReq, &status);

        array2vector(theirBuf, their, count);
        delete[] theirBuf;
        return this->finishDataExchangeTimer();
    }

    size_t getMaxSize() const
    {
        return this->maxSize;
    }
    size_t getMaxSize(const size_t ourSize)
    {
        MPI_Allreduce(&ourSize, &this->maxSize, 1, MPI_UNSIGNED_LONG, MPI_MAX,
                      MPI_COMM_WORLD);
        return this->maxSize;
    }

    int getRank() const
    {
        return this->rank;
    }
    int getProcCount() const
    {
        return this->procCount;
    }

    double startSortTimer()
    {
        this->sortTimer = new MPITimer();
        return this->sortTimer->start();
    }
    double finishSortTimer()
    {
        return this->sortTimer->finish();
    }

    double startParallelSortTimer()
    {
        this->parallelSortTimer = new MPITimer();
        return this->parallelSortTimer->start();
    }
    double finishParallelSortTimer()
    {
        return this->parallelSortTimer->finish();
    }

    double startDataExchangeTimer()
    {
        this->dataExchangeTimer = new MPITimer();
        return this->dataExchangeTimer->start();
    }
    double finishDataExchangeTimer()
    {
        return this->dataExchangeTimer->finish();
    }
};

#endif /* end of include guard: __MPIWRAPPER_H__ */
