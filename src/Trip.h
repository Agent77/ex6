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
    bool calc;
    bool validPath;
    vector<Coordinate*> path;
    pthread_t pathCalc;
//sdf
public:
    Trip();
    Trip(int tripId, int xStart, int yStart, int xEnd, int yEnd,
         int numOfPassengers, double tariff, int time);
    Trip(Trip* t);
    ~Trip();
    /***********************************************************************
* function name: CalculatePath										   *
* The Input: none      		                                           *
* The output: valid/invalid path                           	           *
* The Function operation: calcultes path from start to end of trip	   *
***********************************************************************/
    bool calculatePath();
    /***********************************************************************
* function name: getNextInPath										   *
* The Input: none                                		               *
* The output: next location                               	           *
* The Function operation: pulls the next location from vector, deletes
* it, and returns it                                                    *
***********************************************************************/
    Point* getNextInPath();
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
    int getSizeOfPath();
    bool pathCalculated();
    void isCalculated(int i);
    void setPath(vector<Coordinate*> p);
    void setInvalid() {
        validPath = false;
    }
    bool hasValidPath() {
        return validPath;
    };
    void setThreadId(int id);
    void setMap(Graph* g);


};


#endif //EX2_TRIP_H