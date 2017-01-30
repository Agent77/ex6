

#include "Trip.h"

Trip::Trip() {
    xStart = 0;
    yStart = 0;
    yEnd = 0;
    xEnd = 0;
    startTime = 0;
    tripId = 0;
}

/*
 * non-default constructor
 */
Trip::Trip(int tripId, int xStart, int yStart, int xEnd, int yEnd, int numOfPassengers, double tariff, int time) {
    this->tripId=tripId;
    this->xStart=xStart;
    this->yStart=yStart;
    this->xEnd=xEnd;
    this->yEnd=yEnd;
    this->start =   Point(xStart, yStart);
    this->end =   Point(xEnd, yEnd);
    this->numOfPassengers=numOfPassengers;
    this->tariff = tariff;
    this->startTime = time;
    this->calc = false;
    validPath = true;
}

/*
 * copy constructor for Trip.
 */
Trip::Trip(Trip* t) {
    this->tripId = t->getId();
    this-> xStart = t->getStartX();
    this-> xEnd = t->getEndX();
    this-> yStart = t->getStartY();
    this->yEnd = t->getEndY();
    this->startTime = t->getTripTime();
    this->calc = false;
    validPath = true;
}

void calculateTrip() {

}

Trip::~Trip() {

}

int Trip::getStartX() {
    return xStart;
}
int Trip::getStartY() {
    return yStart;
}

int Trip::getEndX() {
    return xEnd;
}

int Trip::getEndY() {
    return yEnd;
}

int Trip::getId() {
    return tripId;
}

int Trip::getMeters() {
    return metersPassed;
}

int Trip::getNumOfPassengers() {
    return numOfPassengers;
}

double Trip::getTariff() {
    return tariff;
}

Point Trip::getStart() {
    Point p = Point(xStart, yStart);
    return p;
}
Point Trip::getEnd() {
    Point p = Point(xEnd, yEnd);
    return p;
}

void Trip::addMeters() {
    metersPassed =+1;
}

bool Trip::pathCalculated() {
    return calc;
}

void Trip::isCalculated(int i) {
    calc = true;
}

/*
* each time the driver moves, the start point of its trip is updated.
*/
void Trip::updateStartPoint(Point p){
    xStart = p.getCoordinates()[0];
    yStart = p.getCoordinates()[1];
}

int Trip::getTripTime() {
    return startTime;
}

void Trip::setPath(vector<Coordinate *> p) {
    path.swap(p);
}

void Trip::setMap(Graph* g) {
    gps = g;
}

Point* Trip::getNextInPath() {
    Point* p = (Point*)path.front();
    path.erase(path.begin());
    return p;
}

void Trip::setThreadId(int id) {
    threadId = id;
}

int Trip::getSizeOfPath() {
    return path.size();
}

bool Trip::calculatePath() {
    int xSize= gps->getSizeX();
    int ySize = gps->getSizeY();
    Graph* copyGraph = new Grid(gps);
    BFS bfs =  BFS(copyGraph);
    vector<Coordinate*> path;
    Coordinate *start;
    Coordinate *end;
    int x = this->getStartX();
    int y = this->getStartY();
    start = new Point(x, y);
    x = this->getEndX();
    y = this->getEndY();
    end = new Point(x, y);
    path = bfs.getFullPath(start, end);
    Coordinate* p = path.front()->getMyLocation();
    if(p->getX() == -1) {
        return false;
    }
    this->setPath(path);
    delete p;
    delete copyGraph;
    delete start;
    delete end;
    return true;
}