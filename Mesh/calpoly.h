#ifndef CALPOLY_H
#define CALPOLY_H


#include <utility>
#include <vector>

#include "header.h"


class CalPoly
{

private:
    float x;  //
    float y;
    std::vector< Point > pointList;

public:
    CalPoly(float x, float y);
    void AddDirectedLine(Point nextPoint);
    std::vector<Point > getPointList() const;
    void AddPointBFThis(int position, Point addPoint);
    float getX() const;
    float getY() const;
    Point findFirstPoint() const;
};

#endif // CALPOLY_H
