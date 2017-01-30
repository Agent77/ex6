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
    delete myTrip;
    myTrip =  t;
    hasTripAlready = true;
}


Driver::~Driver() {
   // delete myTrip;
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