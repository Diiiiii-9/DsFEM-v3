#ifndef CALCIRCLE_H
#define CALCIRCLE_H

#include <cassert>
#include <vector>
#include "header.h"

class CalCircle
{
private:
    float x;
    float y;
    float r;
    std::vector<float> points;

public:
    CalCircle(float x,float y,float r);
    std::vector<float> getPoints() const;
    void setPoints(const std::vector<float> &newPoints);
    void addPoints(float newPoint);
    std::vector<float> addNPointsEvenly(int N);
    float getX() const;
    float getY() const;
    float getR() const;
    Point findFirstPoint() const;
    std::vector<Point> getPointsbyPoint() const;


};

#endif // CALCIRCLE_H
