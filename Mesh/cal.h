#ifndef CAL_H
#define CAL_H

#include "header.h"
#include <vector>


class Cal
{
private:
    std::vector<Point> pointsList;

public:
    Cal();
    std::vector<Point> getPointsList() const;
    void setPointsList(const std::vector<Point> &newPointsList);
    Point findFirstPoint() const;
};

#endif // CAL_H
