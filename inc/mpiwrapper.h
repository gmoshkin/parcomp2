#ifndef __MPIWRAPPER_H__
#define __MPIWRAPPER_H__

#include <map>
#include <mpi.h>
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>

#include "mpitimer.h"
#include "logger.h"
#include "utils.h"

#define MPILOG(logger, msg) \
    do {\
        std::stringstream ss;\
        ss << msg;\
        logger.log(ss.str());\
    } while(0);

using std::stringstream;
using std::vector;
using std::string;
using std::endl;

class MPIWrapper
{
private:
    typedef std::map<string, MPITimer> timers_t;
    typedef timers_t::const_iterator timers_it;

    int argc;
    char **argv;
    size_t maxSize;
    int rank, procCount;
    Logger *logger;

    timers_t timers;

public:
    MPIWrapper(int argc, char **argv) : argc(argc), argv(argv), maxSize(0),
        rank(-1), procCount(0)
    {
        MPI_Init(&this->argc, &this->argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
        MPI_Comm_size(MPI_COMM_WORLD, &this->procCount);

        stringstream name;
        name << "log_" << this->getRank();
        this->logger = new Logger(name.str());
        this->logger->logTime("Start");

        this->startTimer("total");
    }
    ~MPIWrapper()
    {
        this->logger->logTime("Finish");

        MPI_Finalize();
        if (this->logger != NULL) {
            delete this->logger;
        }
    }

    void log(const string &msg)
    {
        if (this->logger != NULL) {
            this->logger->log(msg);
        }
    }

    void logTimer(const string &name)
    {
        stringstream ss;
        ss << std::fixed << std::setprecision(5);
        ss << name << ": {" << this->timers[name] << " }" << endl;
        this->log(ss.str());
    }

    void logTimers()
    {
        this->finishTimer("total");
        stringstream ss;
        ss << std::fixed << std::setprecision(5);
        for (timers_it it = this->timers.begin(); it != this->timers.end(); it++) {
            ss << it->first << ": { " << it->second << " }" << endl;
        }
        this->log(ss.str());
    }

    template <typename T>
    double exchangeData(int theirRank, const vector<T> &our, vector<T> &their)
    {
        this->startTimer("data exchange");
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
        return this->finishTimer("data exchange");
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

    double startTimer(const string &name)
    {
        return this->timers[name].start();
    }
    double finishTimer(const string &name, bool erase = false)
    {
        double time = this->timers[name].finish();
        if (erase) {
            this->logTimer(name);
            this->timers.erase(name);
        }
        return time;
    }
};

#endif /* end of include guard: __MPIWRAPPER_H__ */
