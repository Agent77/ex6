#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <boost/serialization/access.hpp>
#include "Point.h"

/*
 * Node is a class that represents each object on the grid. it has a reference to the node that
 * we came from on the grid, it's location and a member that says whether it has been visited
 * on the grid or not.
 */

class Node {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & myLocation;
        ar & prev;
        ar & visited;
        ar & obstacle;
    }
private:
    Coordinate* myLocation;
    Node* prev;
    bool visited;
    bool obstacle;
public:
    Node* neighbors;
    Node();
    Node(Node* n);
    ~Node();
    Node(Coordinate* c);
    bool isVisited();
    bool isObstacle();
    void visit();
    void makeObstacle();
    Node* getPrev();
    void setPrev(Node& n);
    int* getLocation();
    Coordinate* getMyLocation();
    void resetNode();
};

#endif //EX1_NODE_H