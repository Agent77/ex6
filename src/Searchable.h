
#ifndef GTESTEX3_SEARCHABLE_H
#define GTESTEX3_SEARCHABLE_H

#include <boost/serialization/access.hpp>
#include <queue>
#include "Graph.h"
#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>
/*
* general class that searches, in no partcilar order,
* a given grid.
*/
class Searchable {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & myDeque;
        ar & source;
        ar & graph;
        ar & destination;
    }

protected:
    std::queue<Node*> myDeque;
    Node* source;
    Graph* graph;
    Node* destination;
public:
    Searchable() {};
    ~Searchable() {};
    Searchable(Graph* g) {
        graph = g;
    };
    /***********************************************************************
	* function name: getPath											   *
	* The Input: none		                                               *
	* The output: none                                  		           *
	* The Function operation: function to get entire path from bfs		   *
	***********************************************************************/
    virtual void getPath() = 0;
    virtual void PrintPath(Node* s, Node* d) = 0;
    /***********************************************************************
	* function name: visitNeighbors										   *
	* The Input: node to visit its neighbors        		               *
	* The output: vector of all that nodes neighbors        	           *
	* The Function operation: sending the input data to the socket         *
	* who connect to this socket.										   *
	***********************************************************************/
    virtual std::vector<Node*> visitNeighbors(Node* n) = 0;
    virtual Node* getDest(){};
    virtual Node* getSource(){};
    virtual vector<Coordinate*> getFullPath(Coordinate* sLoc, Coordinate* dLoc) = 0;
    /***********************************************************************
	* function name: getNextInPath										   *
	* The Input: start and end of path                  	               *
	* The output: next location for driver              		           *
	* The Function operation: gets the next point in bfs path			   *
	***********************************************************************/
};


#endif //GTESTEX3_SEARCHABLE_H
