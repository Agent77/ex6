#ifndef EX2_DRIVER_H
#define EX2_DRIVER_H

#include <vector>
#include <boost/serialization/access.hpp>
#include "Trip.h"
//#include "Passenger.h"
#include "Taxi.h"
/*
 * Driver class represents a driver. It holds the driver fields and
 * in charge of moving on the graph.
 */
class Driver {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & driverId;
        ar & age;
        ar & maritalStatus;
        ar & exp;
        ar & vehicleId;
    }
private:
    int driverId;
    int age;
    int numberOfcustomers;
    int exp;
    int vehicleId;
    double totalRate;
    double avgSatisfaction;
    char maritalStatus;
    Graph* gps;
    Taxi taxi;
    Trip* myTrip;
    bool hasTripAlready;
   // std::vector <Passenger> myPassengers;

public:
    Driver();
    ~Driver();
    Driver(int driverId, int age, char mStatus, int exp, int vehicleId);
    int getAge();
    int getVehicleId();
    int getDriverId();
    int getExp() {
        return exp;
    };
    static void* driveNow(void* d);
    double getSatisfaction();
    Taxi getTaxi();
    Trip* getTrip();
    char getMaritalStatus();
    void setTaxi(Taxi t);
    void setTrip(Trip* t);
    void rateMe(int rating);
    void drive();
    bool arrived();
    void setMap(Graph* map);
    bool hasTrip();
    void eraseTrip();
};

#endif //EX2_DRIVER_H
