#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "point.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::setprecision;
using std::stringstream;

using namespace Batcher;

const char *usage =
"Usage: data2csv in.data [out.csv]\n"
"\n"
"in case n2 != 0:\n"
"   in.data      input binary file\n"
"   out.csv      output file in csv format\n";

int parseArguments(int argc, char *argv[], string &input, string &output)
{
    if (argc < 2) {
        cerr << usage;
        return -1;
    }
    input = argv[1];
    if (argc > 2) {
        output = argv[2];
    } else {
        output = "";
    }
    return 0;
}

void readData(const string &filename, points_t &data)
{
    ifstream in(filename.c_str(), ifstream::in | ifstream::binary);
    size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));
    data.reserve(count);

    // There's got to be a better way of doing this
    for(unsigned int i = 0; i < count; i++) {
        Point point;
        in.read(reinterpret_cast<char*>(&point), sizeof(point));
        data.push_back(point);
    }
}

void writeData(const string &filename, const points_t &data)
{
    if (filename.empty()) {
        cout << setprecision(10);
        for(points_it it = data.begin(); it != data.end(); it++) {
            cout << it->getIndex() << ',' << it->getX() << ',' << it->getY() << endl;
        }
    } else {
        ofstream out(filename.c_str(), ifstream::out);
        out << setprecision(10);

        // There's got to be a better way of doing this
        size_t count = 0;
        for(points_it it = data.begin(); it != data.end(); it++) {
            out << it->getIndex() << ',' << it->getX() << ',' << it->getY() << endl;
            count++;
        }
        cout << "Wrote " << count << " records to " << filename << endl;
    }
}

int main(int argc, char *argv[])
{
    string input, output;
    if (parseArguments(argc, argv, input, output) < 0) {
        return -1;
    }
    points_t data;
    readData(input, data);
    writeData(output, data);
    return 0;
}
