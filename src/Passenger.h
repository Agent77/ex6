#ifndef EX2_PASSENGER_H
#define EX2_PASSENGER_H

#include "Point.h"
#include <boost/serialization/access.hpp>
/*
 * Passenger class which have a location.
 */
class Passenger {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & source;
        ar & dest;
    }
private:
    Point source;
    Point dest;
public:
    Passenger() {};
    Passenger(Point p, Point p2);
    ~Passenger() {}
    int rateDriver();
    Point getSource();
    Point getDestination();
};


#endif //EX2_PASSENGER_H
