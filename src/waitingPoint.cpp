//
// Created by Tiki Lobel on 1/18/17.
//

#include "waitingPoint.h"
waitingPoint::waitingPoint(Point p){
    point= Point(p.getX(), p.getY());
}

void waitingPoint::addDriver(int id) {
    waitingDrivers.push_back(id);
}

void waitingPoint::deleteDriver() {
    waitingDrivers.erase(waitingDrivers.begin());
}

bool waitingPoint::isNextDriver(int id) {
    if(waitingDrivers[0] == id){
        return true;
    }
    return false;
}

Point waitingPoint::getPoint() {
    return point;
}

bool waitingPoint::isEmpty() {
    return waitingDrivers.empty();

}