#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "point.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;

using namespace Batcher;

const char *usage =
"Usage: csv2data in.csv out.data [n]\n"
"\n"
"in case n2 != 0:\n"
"   in.csv       input file in csv format\n"
"   out.data     output binary file\n"
"   n            the number of parts to devide the data into\n";

int parseArguments(int argc, char *argv[], string &input, string &output, int &n)
{
    if (argc < 3) {
        cerr << usage;
        return -1;
    }
    input = argv[1];
    output = argv[2];
    n = 0;
    if (argc > 3) {
        n = atoi(argv[3]);
    }
    return 0;
}

void readData(const string &filename, points_t &data)
{
    ifstream fs(filename.c_str());
    int index;
    float x, y;
    char comma;
    while(fs.good()) {
        fs >> index;
        fs >> comma;
        fs >> x;
        fs >> comma;
        fs >> y;
        if (fs.good()) {
            data.push_back(Point(x, y, index));
        }
    }
    cout << "Read " << data.size() << " records." << endl;
    cout << "First: " << data[0] << endl;
    cout << "Last: " << data[data.size() - 1] << endl;
}

void writeSingle(const string &filename, const points_t &data, size_t start, size_t count)
{
    ofstream out(filename.c_str(), ifstream::out | ifstream::binary);
    out.write(reinterpret_cast<char *>(&count), sizeof(count));

    // There's got to be a better way of doing this
    for(size_t i = start; i < start + count; i++) {
        out.write(reinterpret_cast<const char*>(&(data[i])), sizeof(Point));
    }
    cout << "Wrote " << count << " records to " << filename << endl;
}

void writeMultiple(const string &prefix, const points_t &data, size_t n)
{
    size_t total = 0;
    size_t division = data.size() / n;
    size_t remainder = data.size() % n;
    for (size_t i = 0; i < n; i++) {
        int current = division;
        if (i < remainder) {
            current += 1;
        }
        stringstream ss;
        ss << prefix << i;
        writeSingle(ss.str(), data, total, current);
        total += current;
    }
    if (total != data.size()) {
        cerr << "Something went terribly wrong!" << endl;
    }
}

void writeData(const string &filename, const points_t &data)
{
    ofstream out(filename.c_str(), ifstream::out | ifstream::binary);
    size_t count = data.size();
    out.write(reinterpret_cast<char *>(&count), sizeof(count));

    // There's got to be a better way of doing this
    count = 0;
    for(points_it it = data.begin(); it != data.end(); it++) {
        out.write(reinterpret_cast<const char*>(&(*it)), sizeof(Point));
        count++;
    }
    cout << "Wrote " << count << " records to " << filename << endl;
}

int main(int argc, char *argv[])
{
    string input, output;
    int count;
    if (parseArguments(argc, argv, input, output, count) < 0) {
        return -1;
    }
    points_t data;
    readData(input, data);
    if (count > 0) {
        writeMultiple(output, data, count);
    } else {
        writeData(output, data);
    }
    return 0;
}
