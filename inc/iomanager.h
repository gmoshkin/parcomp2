#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <fstream>

#include "point.h"

using std::string;
using std::ifstream;
using std::ofstream;

using namespace Batcher;

class IOManager
{
private:
    string inputFileName;
    string outputFileName;

public:
    IOManager(const string &input, const string &output)
        : inputFileName(input), outputFileName(output) {}

    void readData(points_t &points)
    {
        ifstream in(this->inputFileName.c_str(), ifstream::in | ifstream::binary);
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
    void writeData(const points_t &points)
    {
        ofstream out(this->outputFileName.c_str(), ifstream::out | ifstream::binary);
        size_t count = points.size();
        out.write(reinterpret_cast<char *>(&count), sizeof(count));

        // There's got to be a better way of doing this
        for(points_it it = points.begin(); it != points.end(); it++) {
            out.write(reinterpret_cast<const char*>(&(*it)), sizeof(Point));
        }
    }
};

#endif /* end of include guard: __CONFIG_H__ */
