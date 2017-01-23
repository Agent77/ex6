#ifndef EX1_COORDINATE_H
#define EX1_COORDINATE_H
#include <boost/serialization/access.hpp>
#include <iostream>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>


/*
 * Abstract class to represent an objects location,
 * based on any number of coordinates given, using an
 * array of locations (x,y,z,etc...). This allows
 * us to handle any dimension wanted by the user.
 */
using namespace std;
class Coordinate {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & locs;
    }

public:
    Coordinate(){};
    Coordinate(Coordinate* p);
    Coordinate(int* a);
    ~Coordinate(){};
    virtual int* getCoordinates();
    virtual int getX(){};
    virtual int getY(){};
    virtual int getNextCoordinate(int place);
    friend std:: ostream& operator<<(std :: ostream& out, Coordinate& t);
    virtual void printFormat(ostream& o) =0;
    virtual bool equalTo(Coordinate* c){};
    virtual Coordinate* getMyLocation() = 0;
    virtual bool equal(Coordinate *)=0;
protected:
    int locs[2];
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Coordinate);
#endif //EX1_COORDINATE_H