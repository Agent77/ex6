//
// Created by Tiki Lobel on 11/17/16.
//

#ifndef GTESTEX3_BFS_H
#define GTESTEX3_BFS_H
#include <queue>
#include "Grid.h"
#include "Searchable.h"
#include <boost/serialization/access.hpp>

/*
 * Class which performs the Breadth-First Search on any given graph/diagram.
 * It contains the graph, start, and end-point, in Node pointer form.
 */
using namespace std;
class BFS: public Searchable {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Searchable>(*this);
    }


public:
    BFS(){};
    BFS(Graph* g);
    Node* getSource();
    Node* getDest();
    void getPath();
    void PrintPath(Node* s, Node* d);
    std::vector<Node*> visitNeighbors(Node* n);
    vector<Coordinate*> getFullPath(Coordinate* sLoc, Coordinate* dLoc);

};

#endif //EXONE_BFS_H