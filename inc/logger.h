#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <string>

class Logger
{
private:
    std::string filename;

public:
    Logger(const std::string &filename) : filename(filename) {}

    void log(const std::string &s)
    {
        std::ofstream fs(this->filename.c_str(), std::ofstream::app);
        fs << s;
    }
};

#endif /* end of include guard: __LOGGER_H__ */
