#include <sstream>
#include "City.h"



City::City() {

}

void City::CallTaxiCenter(Passenger p) {

}
Passenger City::checkForPassengerCalls() {

}

/*
 * The function parses a given string and creates new driver
 */
Driver City::createDriver(string s) {
    int info[6];
    char status;
    int f;
    int i = 0;
    string g;
    std::istringstream sep(s);
    std::getline(sep, g,',');
    std::istringstream (g)>>f;
    for (int j=0; j<(s.size()/2)+1;j++){
        if (i!=2){
            std::istringstream (g)>>f;
            info[i]= f;
        } else{
            status=g[0];
        }
        std::getline(sep, g,',');
        i++;
    }
    Driver d = Driver(info[0], info[1], status, info[3], info[4]);
    return d;
}

/*
 * The function parses a given string and creates new trip
 */
Trip City::createTrip(string s) {
    //7 ints
    int i = 0;
    int value[13];
    int f;
    string g;
    std::istringstream sep(s);
    std::getline(sep, g,',');
    std::istringstream (g)>>f;
    for (int j=0; j<s.size()/2;j++) {
        value[i] = f;
        i++;
        std::getline(sep, g,',');
        std::istringstream (g)>>f;
    }
    Trip trip = Trip(value[0],value[1], value[2], value[3],
                     value[4], value[5], value[6], value[7]);
    return trip;
}

/*
 * The function parses a given string and creates new coordinate
 */
Coordinate* City::createCoordinate(string s) { /*TODO fix*/
    int x = (int)s[0] - 48;
    int y = (int)s[2] - 48;
    Coordinate* point = new Point(x, y);
    return point;
}

/*
 * The function parses a given string and creates new graph
 */
Graph* City::createGraph(string s, string s1) {
    int x1 = stoi(s);
    int y1 = stoi(s1);
    Graph *graphPointer = new Grid(x1, y1);
    return graphPointer;
}

/*
 * The function parses a given string and creates new taxi
 */
Taxi City::createTaxi(string s) {
    int id = (int)s[0] - 48;
    int type = (int)s[2] - 48;
    if(type == 1) {
        StandardCab t = StandardCab(id, type, s[4], s[6]);
        return t;
    } else {
        LuxuryCab t = LuxuryCab(id, type, s[4], s[6]);
        return t;
    }
}