#ifndef __MPITIMER_H__
#define __MPITIMER_H__

#include <mpi.h>
#include <vector>

class MPITimer
{
private:
    typedef std::vector<double> durations_t;

    double startTick;
    double endTick;

    durations_t durations;

public:
    double start()
    {
        return this->startTick = MPI_Wtick();
    }
    double finish()
    {
        this->endTick = MPI_Wtick();
        double duration = this->endTick - this->startTick;
        this->durations.push_back(duration);
        return duration;
    }
    const durations_t &getDurations() const
    {
        return this->durations;
    }
    double getTotal() const
    {
        double total = 0;
        for (durations_t::const_iterator it = this->durations.begin();
             it != this->durations.end(); it++) {
            total += *it;
        }
        return total;
    }
    size_t getCount() const
    {
        this->durations.size();
    }
};

#endif /* end of include guard: __MPITIMER_H__ */
