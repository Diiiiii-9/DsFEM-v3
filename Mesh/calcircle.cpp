#include "calcircle.h"
#include <algorithm>
#include <cmath>


std::vector<float> CalCircle::getPoints() const
{
    return points;
}

void CalCircle::setPoints(const std::vector<float> &newPoints)
{
    points = newPoints;
}

void CalCircle::addPoints(float newPoint) {
    // Check if newPoint is different from any point in points
    for (float point : this->points) {
        if (std::fabs(newPoint - point) < std::numeric_limits<float>::epsilon()) {
            // newPoint is the same as an existing point, so don't add it
            return;
        }
    }

    // newPoint is different from all existing points, so add it
    this->points.push_back(newPoint);
}


//return the vector that has already add N items in
std::vector<float> CalCircle::addNPointsEvenly(int N) {

    std::sort(points.begin(), points.end());

    std::vector<float> newPoints;

    float increment = 2 * M_PI / N;

    for (int i = 0; i < points.size() - 1; i++) {

        float start = points[i];

        newPoints.push_back(start);

        while (true) {

            float intervalLength = points[i+1] - start;

            if (intervalLength > 3 * increment)
            {
                start += increment;
                newPoints.push_back(start);

            } else {

                start = (start + points[i+1]) / 2;
                newPoints.push_back(start);

                break;
            }
        }

    }
    std::sort(newPoints.begin(), newPoints.end());

    //qDebug() << "points:" << points;



    this->points.assign(newPoints.begin(), newPoints.end());

    return this->points;
}


float CalCircle::getX() const
{
    return x;
}

float CalCircle::getY() const
{
    return y;
}

float CalCircle::getR() const
{
    return r;
}

Point CalCircle::findFirstPoint() const
{
    float arctan = this->points.front();
    Point point {static_cast<float>(cos(arctan)), static_cast<float>(sin(arctan))};
    return point;
}

std::vector<Point> CalCircle::getPointsbyPoint() const
{
    std::vector<Point> circlePoints;
    for (const auto& angle : points)
    {
        float _x = this->x + r * std::cos(angle);
        float _y = this->y + r * std::sin(angle);
        circlePoints.emplace_back(Point{_x, _y});
    }
    return circlePoints;
}


CalCircle::CalCircle(float x,float y,float r)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->addPoints(-M_PI);
    this->addPoints(M_PI);
}
