#include "Driver.h"
#include <unistd.h>
#include <iostream>




Driver::Driver() {
    this->driverId=0;
    this->age=0;
    this->maritalStatus='c';
    this->vehicleId=0;
    this->exp=0;
    this->hasTripAlready = false;
    this->myTrip = new Trip(0,0,0,0,0,0,0,0);
}


Driver::Driver(int driverId, int age, char mStatus, int exp, int vehicleId) {
    this->driverId=driverId;
    this->age=age;
    this->maritalStatus=mStatus;
    this->exp=exp;
    this->vehicleId=vehicleId;
    this->hasTripAlready = false;
    this->myTrip = new Trip(0,0,0,0,0,0,0,0);


}

int Driver::getAge() {
    return age;
}

int Driver::getDriverId() {
    return driverId;
}

Taxi Driver::getTaxi() {
    return taxi;
}

Trip* Driver::getTrip() {
    return myTrip;
}

void Driver::setTaxi(Taxi t) {
    taxi = t;
}

void Driver::setTrip(Trip* t) {
    myTrip =  t;
    hasTripAlready = true;
}

/*void Driver::addPassenger(Passenger p) {
    myPassengers.push_back(p);
}*/

Driver::~Driver() {

}

int Driver::getVehicleId() {
     return vehicleId;
}

/*
 * checks if the driver arrived at its destination.
 */
bool Driver::arrived() {
    if (myTrip->getStartX() == myTrip->getEndX()) {
        if(myTrip->getStartY() == myTrip->getEndY()) {
            return true;
        }
    }
    return false;
}

/*
 * sets the drivers map
 */
void Driver::setMap(Graph* map) {
    gps = map;
}

bool Driver::hasTrip() {
    return hasTripAlready;
}

void Driver::eraseTrip() {
    hasTripAlready = false;
}