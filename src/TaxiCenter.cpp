
#include "TaxiCenter.h"


#include "sockets/Udp.h"

using namespace std;


#include "sockets/Udp.h"
#include <pthread.h>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

TaxiCenter::TaxiCenter(Graph* map1) {
    map = map1;
    started = false;
    pathCalculator = new ThreadPool(5);
}
//Checks BFS to find the closest driver to passenger source
Driver TaxiCenter::findClosestDriver(Trip t) {

}

void TaxiCenter::addDriver(Driver d) {
    //, pthread_t* pathCreator
    //pthread_create(pathCreator, NULL, move, (void*)d);
    drivers.push_back(d);

}

bool TaxiCenter::hasDriver(int id) {
    for(int i=0; i<drivers.size();i++) {
        if(drivers[i].getDriverId()==id){
            return true;
        }
    }
    return false;

}

void TaxiCenter::updateMeters() {

}

int TaxiCenter::checkDestinations()  {
}


/*
*finds a driver based on a requested id, and prints their
* current location.
*/

void TaxiCenter::requestDriverLocation(int driverId, vector<Driver> dr){
    int i=0;
    while(dr[i].getDriverId() != driverId) {
        i++;
    }
    Coordinate* p = new Point(dr[i].getTrip()->getStartX(),dr[i].getTrip()->getStartY());
    cout<< *p <<endl;
    delete p;

}

/*
 * Function assigns trips and taxis to drivers.
 * If drivers have already been assigned a trip once from
 * their starting point (0,0), the function pairs up trips
 * with the same ride id as the driver id.
 */
int TaxiCenter::assignDrivers() {

    int count = 0;

    //Assign taxi to driver according to....
    vector<Taxi>::iterator taxi = taxis.begin();
    vector<Driver>::iterator driver = drivers.begin();

    while(taxi != taxis.end()) {
        Taxi currentTaxi = *taxi;
        while((*(driver)).getVehicleId() != (currentTaxi).getId() && driver != drivers.end()) {
            driver++;
        }
        Driver* driverMatch = &(*(driver));
        driverMatch->setTaxi(*taxi);
        taxi++;
    }
    taxis.clear();
    //Meaning drivers already drove away from (0,0)
    if(started) {
        vector<Driver>::iterator driverList = drivers.begin();
        vector<Trip*>::iterator trip;// = trips.begin();
        while(driverList != drivers.end()) {
            trip = trips.begin();
            while (trip != trips.end()) {
                //matching up trips and driver current locations
                if ((*(driverList)).getTrip()->getStartX() == ((*(trip))->getStartX())) {
                    if ((*(driverList)).getTrip()->getStartY() == (*(trip))->getStartY()) {
                        Trip t = (*(trip));
                        (*(driverList)).setTrip((&t));
                    }
                }
                trip++;
            }
            driverList++;
        }

    }
    else {
        //drivers all at (0,0))
        vector<Driver>::iterator driverList = drivers.begin();
        vector<Trip*>::iterator trip = trips.begin();
        while (driverList != drivers.end() && trip != trips.end()) {
            (*(driverList)).setTrip((*(trip)));
            driverList++;
            trip++;
            count++;
        }
        //So function knows the drivers have already been assigned a trip before
        started = true;

    }
    trips.clear();
    return count;

}

void TaxiCenter::assignTrip(Trip t) {
/*    vector<Driver>::iterator driverIter = drivers.begin();
    while((*(driverIter)).getTrip().getEnd() !=  NULL) {
        driverIter++;
    }
    (*(driverIter)).setTrip(t);*/
}
vector <Driver> TaxiCenter::getDrivers (){
    return drivers;
}


/*
 * adds taxi to vector of taxis
 */
void TaxiCenter::addTaxi(Taxi t) {
    taxis.push_back(t);
}


vector<Trip*> TaxiCenter::getTrips() {
    return trips;
}

pthread_t* TaxiCenter::getTripCalculator(int id) {
    return pathCalculator->getThread(id);

}
bool TaxiCenter::addTrip(Trip* t) {
    //t->setMap(map);

    if(t->getSizeOfPath() == 0) {
        pathCalculator->addTripToCalculate(t);
    }
    while(!t->pathCalculated()) {

    }
    if(t->hasValidPath()) {
        trips.push_back(t);
        return true;
    } else {
        cout << "-1"<<endl;
        return false;
    }
}

void TaxiCenter::addDriver(int driverId, int age, char mStatus, int exp, int vehicleId) {
    Driver *d = new Driver (driverId, age, mStatus, exp, vehicleId);
    drivers.push_back(*d);
}

/*
 * updates the driver's trip
 */
void TaxiCenter::updateDriverTrip(Trip newTrip, int place){
    drivers.at(place).setTrip(&newTrip);
}

void TaxiCenter::updateDriverTrip(Trip newTrip){
    vector<Driver>::iterator driver = drivers.end();
    (*(driver)).setTrip(&newTrip);
}

Taxi TaxiCenter::assignTaxi(int driverId){
    vector<Driver>::iterator driver = drivers.begin();
    vector<Taxi>::iterator taxi = taxis.begin();
    while ((*(driver)).getDriverId()!=driverId){
        driver++;
        taxi++;

    }
    (*(driver)).setTaxi((*(taxi)));
    return *(taxi);
}

/*
 * searches through the trips to find which trip has the same
 * start time as the time that was sent to function
 */
Trip* TaxiCenter::getNextTrip(int currentTime) {
    vector<Trip*>::iterator trip = trips.begin();
    while (trip != trips.end() && (*(trip))->getTripTime() > currentTime) { //TODO does this stop loop
        trip++;
    }
    Trip* nextTrip = *(trip);
    //removes trip from possible trips to assign in future
    trips.erase(trip);
    return nextTrip;
}

/*
 * checks how many trips we are expecting to
 * assign at a certain time.
 */
int TaxiCenter::checkTripTimes(int currentTime) {
    int counter =0;
    for (int i = 0; i < trips.size();i++) {
        if (trips[i]->getTripTime() == currentTime) {
            counter++;
        }
    }
    return counter;
}

Graph* TaxiCenter::getMap() {
    return map;
}

void TaxiCenter::resetDrivers(vector<Driver> ds) {
    drivers = ds;
}

/*
 * deletes a driver from the taxi center,
 * used when the driver has finished its first trip
 * and is waiting to be assigned another one,
 * and is then added back to taxi center
 */
void TaxiCenter::deleteDriver(int id) {
    int i=0;
    while(drivers[i].getDriverId() != id) {
        i++;
    }
    drivers.erase(drivers.begin()+i);
}

void TaxiCenter::moveDriver(int id, Point* p) {
    int i = 0;
    while(drivers[i].getDriverId() != id) {
        i++;
    }
    drivers[i].getTrip()->updateStartPoint(p);
}

bool TaxiCenter::hasTaxi(int id) {
    int i;
    for(i=0; i < taxis.size(); i++) {
        if(taxis[i].getId()==id) {
            return true;
        }
    }
    return false;
}

bool TaxiCenter::wdHasDriver(int id, vector<Driver> d) {
    for(int i=0; i<d.size();i++) {
        if(d[i].getDriverId()==id){
            return true;
        }
    }
    return false;
}