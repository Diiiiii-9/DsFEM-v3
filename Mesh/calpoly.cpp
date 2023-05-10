#include "calpoly.h"
#include <utility>

std::vector<Point> CalPoly::getPointList() const
{
    return pointList;
}

float CalPoly::getX() const
{
    return x;
}

float CalPoly::getY() const
{
    return y;
}

Point CalPoly::findFirstPoint() const
{
    return pointList.front();
}

CalPoly::CalPoly(float x,float y)
{
    this->x = x;
    this->y = y;
    //x,y as the first pair
//    Point point {x,y};
//    pointList.push_back(point);
}

void CalPoly::AddDirectedLine(Point nextPoint)
{
    pointList.push_back(nextPoint);
}

void CalPoly::AddPointBFThis(int position, Point addPoint)
{
    pointList.insert(pointList.begin()+position, addPoint);
}

