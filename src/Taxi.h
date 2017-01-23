
#ifndef GTESTEX3_TAXI_H
#define GTESTEX3_TAXI_H

#include <string>
#include "BFS.h"
#include <boost/serialization/access.hpp>

class Taxi {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & cabId;
        ar & cabType;
        ar & kmPassed;
        ar & cabManufacturer;
        ar & cabColor;
        ar & tariff;
        ar & speed;
    }
protected:
    //BFS* gps;
    int cabId;
    int cabType;
    int kmPassed;
    char cabManufacturer;
    char cabColor;
    int tariff;
    int speed;

public:
    Taxi();
    Taxi(int id, int type, char manufacturer, char color);
    ~Taxi(){};
    virtual void updateKms();
    virtual int getSpeed();
    virtual char getManufacturer();
    virtual int getTariff();
    virtual int getId();
    virtual int getKms();
    virtual int getType();
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Taxi);

#endif //EX2_TAXI_H
