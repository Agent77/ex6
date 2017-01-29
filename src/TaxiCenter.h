
#ifndef EX2_TAXICENTER_H
#define EX2_TAXICENTER_H

#include "Trip.h"
#include "StandardCab.h"
#include "LuxuryCab.h"
#include "Driver.h"
#include "Point.h"
#include "ThreadPool.h"
#include <vector>
#include <boost/serialization/access.hpp>

using namespace std;
/*
 * class that controls all drivers/taxis in alerting them
 * to move, assigning trips, and getting locations
 */
class TaxiCenter {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & drivers;
        ar & taxis;
        ar & locations;
        ar & started;
    }
private:
    std::vector<Driver> drivers;
    std::vector<Taxi> taxis;
    Point* locations;
    std::vector<Trip*> trips;
    Graph* map;
    bool started;
    ThreadPool* pathCalculator;
public:
    TaxiCenter(){};
    TaxiCenter(Graph* map1);
    ~TaxiCenter() {
        taxis.clear();
        vector<Taxi>().swap(taxis);
        drivers.clear();
        vector<Driver>().swap(drivers);
        trips.clear();
        //vector<Trip>().swap(trips);
    };
    int assignDrivers();
    Coordinate* driveAll();
    Driver findClosestDriver(Trip t);
    void addDriver(int driverId, int age, char mStatus, int vehicleId, int exp);
    bool addTrip(Trip* t);
    void updateMeters();
    int checkDestinations();
    void requestDriverLocation(int driverId, vector<Driver> dr);
    vector <Driver> getDrivers();
    vector<Trip*> getTrips();
    void addTaxi(Taxi t);
    void addDriver(Driver d);
    void updateDriverTrip(Trip newTrip, int place);
    void updateDriverTrip(Trip newTrip);
    Taxi assignTaxi(int driverId);
    Trip* getNextTrip (int currentTime);
    int checkTripTimes(int currentTime);
    Graph* getMap();
    void assignTrip(Trip t);
    void resetDrivers(vector<Driver> d);
    void deleteDriver(int i);
    bool hasDriver(int id);
    void moveDriver(int id, Point* p);
    pthread_t* getTripCalculator(int id);
    bool hasTaxi(int id);
    bool wdHasDriver(int id, vector<Driver> d);
    bool hasTrip(int id);
};


#endif //EX2_TAXICENTER_H