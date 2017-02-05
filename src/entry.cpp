#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "mpiwrapper.h"
#include "mpisorter.h"
#include "iomanager.h"
#include "logger.h"
#include "utils.h"
#include "point.h"
#include "sort.h"

using namespace Batcher;
using std::cout;
using std::endl;
using std::swap;
using std::string;

static const size_t defaultThreshold = 10000;

int main(int argc, char *argv[])
{
    MPIWrapper mpi(argc, argv);

    string input, output;
    long threshold;
    if (parseFilenames(argc, argv, mpi.getRank(), input, output, threshold) < 0) {
        return -1;
    }
    IOManager iomanager(input, output);
    xpoints_t ourData;
    mpi.startTimer("read data");
    iomanager.readData(ourData);
    mpi.finishTimer("read data", true);

    if (threshold < 0) {
        threshold = defaultThreshold;
    }
    MPISorter<XComparablePoint> sorter(mpi, ourData, threshold);
    sorter.extendSize();
    sorter.sortOur();
    sorter.sortAll();

    mpi.startTimer("write data");
    iomanager.writeData(sorter.getOurData());
    mpi.finishTimer("write data", true);

    mpi.logTimers();

    return 0;
}
