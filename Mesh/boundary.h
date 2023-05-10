#ifndef BOUNDARY_H
#define BOUNDARY_H
#include <iostream>
#include <vector>

#include "header.h"

using namespace std;



class Boundary
{
private:

    //to store every point
    vector<Point> poly;
    bool onLine(line l1, Point p);
    int direction(Point a, Point b, Point c);
    bool isIntersect(line l1, line l2);
    bool checkInside(vector<Point> poly, int n, Point p);
    int test();
    bool isExter = false;
    int bound_direction;

public:
    Boundary(vector<Point> points);
    /**
    *   @author Diiiiii
    *   @returns 0--- no interior either exterior
    *            1--- interior
    *            2--- exterior
    */
    int boundCondition(vector<Point> judgePoints);
    bool getIsExter() const;
    vector<Point> getPoly() const
    {
        return poly;
    }
    /**
    *   @author Diiiiii
    *   @returns 0---Collinear
    *            1---Clockwise
    *            2---AntiClockwise
    */
    int getBound_direction() const;
};

#endif // BOUNDARY_H
