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
    //this->myTrip = NULL;
}


Driver::Driver(int driverId, int age, char mStatus, int exp, int vehicleId) {
    this->driverId=driverId;
    this->age=age;
    this->maritalStatus=mStatus;
    this->exp=exp;
    this->vehicleId=vehicleId;
    this->hasTripAlready = false;
    //this->myTrip = NULL;


}

/*
* using its grid, the driver uses a bfs algorithms to
* continue travelling until it arrives at its destination.
*/
void Driver::drive() {
    /*int xSize= gps->getSizeX();
    int ySize = gps->getSizeY();
    Graph* copyGraph = new Grid(xSize, ySize);
    BFS bfs =  BFS(copyGraph);
    vector<Coordinate*> path;
    Coordinate *start;
    Coordinate *end;
    int x = myTrip.getStartX();
    int y = myTrip.getStartY();
    start = new Point(x, y);
    x = myTrip.getEndX();
    y = myTrip.getEndY();
    end = new Point(x, y);
    path = bfs.getFullPath(start, end);

    myTrip.setPath(path);
    delete copyGraph;
    delete start;
    delete end;*/
}


 void* Driver::driveNow(void* d) {
    Driver* driver = (Driver*)d;
     driver->drive();
     pthread_exit(0);
}
int Driver::getAge() {
    return age;
}

int Driver::getDriverId() {
    return driverId;
}

double Driver::getSatisfaction() {
    return avgSatisfaction;
}

Taxi Driver::getTaxi() {
    return taxi;
}

Trip* Driver::getTrip() {
    return myTrip;
}

char Driver::getMaritalStatus() {
    return maritalStatus;
}

void Driver::setTaxi(Taxi t) {
    taxi = t;
}

void Driver::setTrip(Trip* t) {
    myTrip =  t;
    hasTripAlready = true;

}

/*
* adds another rating and calculates average.
*/
void Driver::rateMe(int rating) {
    numberOfcustomers=+1;
    totalRate+=rating;
    avgSatisfaction =totalRate/numberOfcustomers;
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