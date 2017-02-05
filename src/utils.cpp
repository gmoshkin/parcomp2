#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "utils.h"

using std::cerr;
using std::vector;
using std::string;
using std::stringstream;

const char *usage =
"Usage: bsort n1 n2\n"
"\n"
"in case n2 != 0:\n"
"   n1      the number of elements in the first ordered group\n"
"   n2      the number of elements in the second ordered group\n"
"\n"
"in case n2 == 0:\n"
"   n1      the number of elements in the unordered group\n";

const char *usage2 =
"Usage: bsort input output [threshold]\n"
"\n"
"   input      input file prefix\n"
"   output     output file prefix\n"
"   threshold  the minimal size of the array which has to be sorted with the\n"
"              heap sort\n"
"The respective filenames are constructed by appending processor rank to the\n"
"prefix\n";

void printUsage()
{
    cerr << usage;
}

int parseOptions(int argc, char *argv[], int &n1, int &n2)
{
    if (argc < 3) {
        printUsage();
        return -1;
    }
    n1 = atoi(argv[1]);
    n2 = atoi(argv[2]);
    return 0;
}

int parseLists(int argc, char *argv[], numbers_t &l1, numbers_t &l2)
{
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '.') {
            i++;
            break;
        }
        l1.push_back(atoi(argv[i]));
    }
    for (; i < argc; i++) {
        if (argv[i][0] == '|') {
            break;
        }
        l2.push_back(atoi(argv[i]));
    }
    return 0;
}

std::ostream &operator <<(std::ostream &out, const numbers_t &numbers)
{
    out << "{ ";
    for (numbers_it it = numbers.begin(); it != numbers.end(); it++) {
        out << *it << " ";
    }
    out << "}";
    return out;
}

int parseFilenames(int argc, char *argv[], int rank, string &input, string &output, long &threshold)
{
    if (argc < 3) {
        cerr << usage2;
        return -1;
    }
    stringstream in, out;
    in << argv[1] << rank;
    out << argv[2] << rank;
    input = in.str();
    output = out.str();
    if (argc > 3) {
        std::istringstream converter(argv[3]);
        converter >> threshold;
    } else {
        threshold = -1;
    }
    return 0;
}
