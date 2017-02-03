#ifndef __POINT_H__
#define __POINT_H__

#include <vector>
#include <iostream>
#ifndef WITHOUT_MPI
#include <mpi.h>
#endif

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
#ifndef WITHOUT_MPI
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
#endif
};

struct XComparablePoint: public Point
{
    bool operator <(const Point &that)
    {
        return ((this->index >= 0) &&
                ((that.index < 0) ||
                 (this->getX() < that.getX())));
    }
    XComparablePoint(float x, float y, int i) : Point(x, y, i) {}
    XComparablePoint(const Point &p) : Point(p.getX(), p.getY(), p.getIndex()) {}
};

struct YComparablePoint: public Point
{
    bool operator <(const Point &that)
    {
        return ((this->index >= 0) &&
                ((that.index < 0) ||
                 (this->getY() < that.getY())));
    }
    YComparablePoint(float x, float y, int i) : Point(x, y, i) {}
    YComparablePoint(const Point &p) : Point(p.getX(), p.getY(), p.getIndex()) {}
};

ostream &operator <<(ostream &out, const Point &p)
{
    return out << '{' << p.getX() << ',' << p.getY() << "}[" << p.getIndex() << ']';
}

typedef std::vector<Point> points_t;
typedef points_t::const_iterator points_it;

typedef std::vector<XComparablePoint> xpoints_t;
typedef xpoints_t::const_iterator xpoints_it;

typedef std::vector<YComparablePoint> ypoints_t;
typedef ypoints_t::const_iterator ypoints_it;

}

#endif /* end of include guard: __POINT_H__ */
