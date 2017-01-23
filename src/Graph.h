
#ifndef EX1_GRAPH_H
#define EX1_GRAPH_H

#include <vector>
#include "Node.h"
#include <boost/serialization/access.hpp>



/*
 * Graph is an abstract class which represents any kind of diagram. it has the abstract methods
 * for the diagram that are in charge of providing the information needed.
 */
class Graph {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & sizeX;
        ar & sizeY;
    }

protected:
    Node* arrayOfPtrsToNodes[1000][1000];
    int sizeX;
    int sizeY;
public:
    Graph(){};
    ~Graph() {
         for (int i = 0; i < sizeX; i++) {
            for (int j = 0; j < sizeY; j++) {
               Node* n = arrayOfPtrsToNodes[i][j];
               delete n->getMyLocation();
              delete n;
          }
         }
    };
    virtual Node* getLocationOfPrev(Node* n) {};
    virtual std::vector<Node*> getNeighbors(Node* n)=0;
    virtual Node* getNode(Coordinate* p){};
    virtual void addObstacle(Coordinate* p){};
    virtual void resetGraph(){};
    //virtual void getNodes() {};
    virtual void deleteGraph() = 0;
    virtual int getSizeX()=0;
    virtual int getSizeY()=0;
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Graph);

#endif //EX1_GRAPH_H
