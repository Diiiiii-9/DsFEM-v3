#include "cal.h"

std::vector<Point> Cal::getPointsList() const
{
    return pointsList;
}

void Cal::setPointsList(const std::vector<Point> &newPointsList)
{
    this->pointsList = newPointsList;
}

Point Cal::findFirstPoint() const
{
    return pointsList.front();
}

Cal::Cal()
{

}
