#include <boost/serialization/export.hpp>
#include "Point.h"
using namespace std;
/*
 * Empty constructor
 */
Point::Point() {

}

/*
 * Constructor which receives two points and
 * inserts into the Coordinate array.
 */
Point::Point(int xAxis, int yAxis) {
    locs[0] = xAxis;
    locs[1] = yAxis;
}

/*
 * Overload for print operator, which prints the Point
 * in a specific format.
 */
void Point::printFormat(ostream& o) {
    o << "(" << locs[0] << "," << locs[1] << ")";
}

/*
 * Compares two points and returns true if equal, false if not.
 */
bool Point::equalTo(Coordinate *c) {
    Point point(c);
    bool equal =  (locs[0] == (point.getX()) && (locs[1] == (point.getY())));
    return equal;
}
/*
 * Returns the second number in array, which
 * is always the Y value.
 */
int Point::getY() {
    return locs[1];
}

/*
 * Returns the first number in array, which
 * is always the X value.
 */
int Point::getX() {
    return locs[0];
}

/*
 * Another constructor which accepts a Coordinate,
 * and takes its coordinates to create a Point object.
 */
Point::Point(Coordinate *cor) {
    int size = sizeof(locs)/4;
    for(int i = 0; i< 2; i++) {
        locs[i] = cor->getNextCoordinate(i);
    }
}

/*
 * Destructor for Point class.
 */
Point::~Point() {

}

bool Point::equal(Coordinate *cor) {
    if(cor->getX() == locs[0] && cor->getY() == locs[1]) {
        return true;
    }
    return false;
}

Coordinate* Point::getMyLocation() {
    Coordinate* p = new Point(locs[0], locs[1]);
    return p;
}
BOOST_CLASS_EXPORT(Point);