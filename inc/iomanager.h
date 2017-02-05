#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>
#include <fstream>

#include "point.h"

using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;

using namespace Batcher;

class IOManager
{
private:
    string inputFileName;
    string outputFileName;

public:
    IOManager(const string &input, const string &output)
        : inputFileName(input), outputFileName(output) {}

    template <typename T>
    void readData(vector<T> &points)
    {
        ifstream in(this->inputFileName.c_str(), ifstream::in | ifstream::binary);
        if (in.fail()) {
            cerr << "Couldn't open " << this->inputFileName << endl;
        }
        size_t count = 0;
        in.read(reinterpret_cast<char *>(&count), sizeof(count));
        points.reserve(count);

        // There's got to be a better way of doing this
        for(unsigned int i = 0; i < count; i++) {
            Point point;
            in.read(reinterpret_cast<char*>(&point), sizeof(point));
            points.push_back(point);
        }
    }
    template <typename T>
    void writeData(const vector<T> &points)
    {
        typedef typename vector<T>::const_iterator points_it;
        ofstream out(this->outputFileName.c_str(), ifstream::out | ifstream::binary);
        if (out.fail()) {
            cerr << "Couldn't open " << this->outputFileName << endl;
        }
        size_t count = points.size();
        out.write(reinterpret_cast<char *>(&count), sizeof(count));

        // There's got to be a better way of doing this
        for(points_it it = points.begin(); it != points.end(); it++) {
            out.write(reinterpret_cast<const char*>(&(*it)), sizeof(Point));
        }
    }
};

#endif /* end of include guard: __CONFIG_H__ */
