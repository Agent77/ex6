

#ifndef EX1_POINT_H
#define EX1_POINT_H
#include <iostream>
#include "Coordinate.h"
#include <boost/serialization/access.hpp>
/*
 * Point class is a specific implementation of
 * the Coordinate abstract class. It has an array of size
 * 2, for x and y, so is used for 2D graphs.
 */
class Point: public Coordinate {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Coordinate>(*this);
    }

public:
    Point();
    ~Point();
    Point(int xAxis, int yAxis);
    void printFormat(ostream& o);
    bool equalTo(Coordinate* c);
    friend bool operator== (Point const& p, Point const& p2);
    int getX();
    Point(Coordinate* cor);
    int getY();
    bool equal(Coordinate* c);
    Coordinate* getMyLocation();

};
#endif //EXONE_POINT_H