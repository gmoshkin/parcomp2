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
    iomanager.readData(ourData);

    MPISorter<XComparablePoint> sorter(mpi, ourData);
    sorter.extendSize();
    sorter.sortOur();
    sorter.sortAll();

    iomanager.writeData(sorter.getOurData());

    mpi.logTimers();

    iomanager.writeData(sorter.getOurData());

    return 0;
}
