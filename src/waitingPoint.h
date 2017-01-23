//
// Created by Tiki Lobel on 1/18/17.
//

#ifndef EX4_WAITINGPOINT_H
#define EX4_WAITINGPOINT_H


#include "Driver.h"

class waitingPoint {
private:
    Point point;
    vector<int> waitingDrivers;
public:
    waitingPoint(Point p);
    ~waitingPoint(){};
    void addDriver(int id);
    void deleteDriver();
    bool isNextDriver(int id);
    Point getPoint();
    bool isEmpty();
};


#endif //EX4_WAITINGPOINT_H
