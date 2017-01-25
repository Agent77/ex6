//
// Created by Tiki Lobel on 11/29/16.
//

#ifndef EX2_CITY_H
#define EX2_CITY_H
#include "TaxiCenter.h"
#include "Passenger.h"
#include <boost/serialization/access.hpp>

/*
* City parses and returns requested object.
*/

class City {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        //ar & tc;
    }

private:
    //TaxiCenter tc;
    int gridSizeX;
    int gridSizeY;
public:
    City();
    void CallTaxiCenter(Passenger p);
    Passenger checkForPassengerCalls();
   // TaxiCenter getTaxiCenter() {};
    Driver createDriver(string s);
    Trip* createTrip(string s);
    Graph* createGraph(string s);
    Taxi createTaxi(string s);
    Coordinate* createCoordinate(string obstacleLocation);
    bool isValidStatus(char c);
    bool isNumber(string s);
    bool validTaxi(string s);
    bool validTrip(string s);
};


#endif //EX2_CITY_H
