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


int main(int argc, char *argv[])
{
    MPIWrapper mpi(argc, argv);

    string input, output;
    if (parseFilenames(argc, argv, mpi.getRank(), input, output) < 0) {
        return -1;
    }
    IOManager iomanager(input, output);
    xpoints_t ourData;
    mpi.startTimer("read data");
    iomanager.readData(ourData);
    mpi.finishTimer("read data");

    MPISorter<XComparablePoint> sorter(mpi, ourData);
    sorter.extendSize();
    sorter.sortOur();
    sorter.sortAll();

    mpi.startTimer("write data");
    iomanager.writeData(sorter.getOurData());
    mpi.finishTimer("write data");

    mpi.logTimers();

    return 0;
}
