#include "boundary.h"
#include <QMessageBox>
#include <bits/stdc++.h>
#include <cmath>

Boundary::Boundary(vector<Point> points)
{
    this->poly = points;


    //1. Set the direction
    for (int i = 0; i < int(points.size()-2); i++) {

        this->bound_direction = this->direction(points.at(i), points.at(i+1), points.at(i+2));

        if(this->bound_direction != 0)
            break;

    }

//    //2. Set the circle line
//    list<line> line_Dlist;

//    //2.1 for the first n-1 element
//    for (int i = 0; i < points.size()-1; ++i) {
//        line _line {points.at(i), points.at(i+1)};
//        line_Dlist.push_back()

//    }

//    //2.2 for the last point
//    line _line {points.back(), points.front()};
//    line_Dlist.push_back(_line);


}

bool Boundary::onLine(line l1, Point p)
{
    // Check whether p is on the line or not
    if (p.x <= fmax(l1.p1.x, l1.p2.x)
            && p.x <= fmin(l1.p1.x, l1.p2.x)
            && (p.y <= fmax(l1.p1.y, l1.p2.y)
            && p.y <= fmin(l1.p1.y, l1.p2.y)))
        return true;

    return false;
}

/**
*   @author Diiiiii
*   @returns 0---Collinear
*            1---Clockwise
*            2---AntiClockwise
*/
int Boundary::direction(Point a, Point b, Point c)
{
    float val = (b.y - a.y) * (c.x - b.x)
              - (b.x - a.x) * (c.y - b.y);

    if (val == 0)

        // Collinear
        return 0;

    else if (val < 0)

        // Anti-clockwise direction
        return 2;

    // Clockwise direction
    return 1;
}

bool Boundary::isIntersect(line l1, line l2)
{
    // Four direction for two lines and points of other line
    int dir1 = direction(l1.p1, l1.p2, l2.p1);
    int dir2 = direction(l1.p1, l1.p2, l2.p2);
    int dir3 = direction(l2.p1, l2.p2, l1.p1);
    int dir4 = direction(l2.p1, l2.p2, l1.p2);

    // When intersecting
    if (dir1 != dir2 && dir3 != dir4)
        return true;

    // When p2 of line2 are on the line1
    if (dir1 == 0 && onLine(l1, l2.p1))
        return true;

    // When p1 of line2 are on the line1
    if (dir2 == 0 && onLine(l1, l2.p2))
        return true;

    // When p2 of line1 are on the line2
    if (dir3 == 0 && onLine(l2, l1.p1))
        return true;

    // When p1 of line1 are on the line2
    if (dir4 == 0 && onLine(l2, l1.p2))
        return true;

    return false;
}

bool Boundary::checkInside(vector<Point> poly, int n, Point p)
{

    // When polygon has less than 3 edge, it is not polygon
    if (n < 3)
        return false;

    // Create a point at infinity, y is same as point p
    line exline = { p, { 9999, p.y } };
    int count = 0;
    int i = 0;
    do {

        // Forming a line from two consecutive points of
        // poly
        line side = { poly[i], poly[(i + 1) % n] };
        if (isIntersect(side, exline)) {

            // If side is intersects exline
            if (direction(side.p1, p, side.p2) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % n;
    } while (i != 0);

    // When count is odd
    return count & 1;
}

// Driver code
int Boundary::test()
{
    vector<Point> polygon{ { 0, 0 }, { 10, 0 }, { 10, 10 }, { 0, 10 } };
    Point p = { 5, 3 };
    int n = 4;

    // Function call
    if (checkInside(polygon, n, p))
        std::cout << "Point is inside.";
    else
        std::cout << "Point is outside.";

    return 0;
}

int Boundary::getBound_direction() const
{
    return bound_direction;
}

bool Boundary::getIsExter() const
{
    return isExter;
}

/**
*   @author Diiiiii
*   @returns 0--- no interior either exterior
*            1--- interior
*            2--- exterior
*/
int Boundary::boundCondition(vector<Point> judgePoints)
{
    bool isInterior = false;
    bool isExterior = false;

    //if there's no hole
    if(judgePoints.empty())
    {
        this->isExter = true;
        return 2;
    }


    // 1.
    for(Point point : judgePoints)
    {

        if(checkInside(this->poly, poly.size(), point))
            isExterior = true;
        else
            isInterior = true;
    }



    if(isInterior && isExterior)
        return 0;
    else if(isInterior && !isExterior)
        return 1;
    else if(!isInterior && isExterior)
    {
        this->isExter = true;
        return 2;
    }

    else {
        cout << "wrong with boundCondition" << endl;
        return -1;

    }

}
