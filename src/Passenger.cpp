#include "Passenger.h"
#include <cstdlib>

Passenger::Passenger(Point p, Point p2) {
    source = p;
    dest = p2;
}
/*
 * rates the driver through a random number between 1 and 5
 */
int Passenger::rateDriver() {
    int rate = rand() %5;
    return rate;
}

Point Passenger::getSource() {
    return source;
}

Point Passenger::getDestination() {
    return dest;
}