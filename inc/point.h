#ifndef __POINT_H__
#define __POINT_H__

#include <vector>
#include <mpi.h>
#include <iostream>

using std::ostream;

namespace Batcher {

struct Point
{
    float coord[2];
    int index;

    Point(float x, float y, int i)
    {
        setX(x);
        setY(y);
        setIndex(i);
    }
    Point() {}

    float getX() const
    {
        return this->coord[0];
    }
    float setX(float x)
    {
        return this->coord[0] = x;
    }
    float getY() const
    {
        return this->coord[1];
    }
    float setY(float y)
    {
        return this->coord[1] = y;
    }
    int getIndex() const
    {
        return this->index;
    }
    int setIndex(int i)
    {
        return this->index = i;
    }
    static void getMPIDatatype(MPI_Datatype &type)
    {
        int count = 2;
        int blockLengths[count] = { 2, 1 };
        MPI_Aint floatExtent;
        MPI_Type_extent(MPI_FLOAT, &floatExtent);
        MPI_Aint dispacements[count] = {
            static_cast<MPI_Aint>(0),
            floatExtent * blockLengths[0]
        };
        MPI_Datatype types[count] = { MPI_FLOAT, MPI_INT };
        MPI_Type_create_struct(count, blockLengths, dispacements, types, &type);
        MPI_Type_commit(&type);
    }
};

struct XComparablePoint: public Point
{
    bool operator <(const Point &that)
    {
        return this->getX() < that.getX();
    }
};

struct YComparablePoint: public Point
{
    bool operator <(const Point &that)
    {
        return this->getY() < that.getY();
    }
};

ostream &operator <<(ostream &out, const Point &p)
{
    return out << '{' << p.getX() << ',' << p.getY() << "}[" << p.getIndex() << ']';
}

typedef std::vector<Point> points_t;
typedef points_t::const_iterator points_it;

}

#endif /* end of include guard: __POINT_H__ */
