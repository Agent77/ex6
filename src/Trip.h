#ifndef EX2_TRIP_H
#define EX2_TRIP_H

#include "Point.h"
#include "Trip.h"
#include "Grid.h"
#include "BFS.h"
#include <boost/serialization/access.hpp>
#include <vector>
class Trip {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & tripId;
        ar & xStart;
        ar & yStart;
        ar & xEnd;
        ar & yEnd;
        ar & numOfPassengers;
        ar & metersPassed;
        ar & tariff;
        ar & startTime;
    }
private:
    int tripId;
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    Point start;
    Point end;
    int numOfPassengers;
    int metersPassed;
    double tariff;
    int startTime;
    int threadId;
    Graph* gps;
    vector<Coordinate*> path;
    pthread_t pathCalc;
//sdf
public:
    Trip();
    Trip(int tripId, int xStart, int yStart, int xEnd, int yEnd, int numOfPassengers, double tariff, int time);
    Trip(Trip* t);
    ~Trip();
    int getId();
    int getMeters();
    int getNumOfPassengers();
    double getTariff();
    void addMeters();
    Point getStart();
    Point getEnd();
    void updateStartPoint(Point p);
    int getTripTime();
    int getStartX();
    int getStartY();
    int getEndX();
    int getEndY();
    void setThreadId(int id);
    int getThreadId();
    void setMap(Graph* g);
    void calculatePath();
    void setPath(vector<Coordinate*> p);
    Point* getNextInPath();
    int getSizeOfPath();
};


#endif //EX2_TRIP_H
