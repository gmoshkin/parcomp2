#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <iostream>

#define DEBUG(x)
#ifndef DEBUG
#   define DEBUG(x) std::cout << x
#endif

typedef std::vector<int> numbers_t;
typedef numbers_t::const_iterator numbers_it;

std::ostream &operator <<(std::ostream &out, const numbers_t &);

template <typename T>
std::ostream &operator <<(std::ostream &out, const std::vector<T> &v)
{
    out << "{ ";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); it++) {
        out << *it << ' ';
    }
    out << '}';
    return out;
}

template <typename T>
const T *vector2array(const std::vector<T> &v)
{
    return &(v[0]);
}

template <typename T>
void array2vector(const T *a, std::vector<T> &v, size_t count)
{
    v.assign(a, a + count);
}

int parseOptions(int argc, char *argv[], int &n1, int &n2);
int parseLists(int argc, char *argv[], numbers_t &n1, numbers_t &n2);
int parseFilenames(int argc, char *argv[], int, std::string &, std::string &, long &threshold);

#endif /* end of include guard: __UTILS_H__ */
