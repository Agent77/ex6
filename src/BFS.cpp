
#include <sstream>
#include "BFS.h"


using namespace std;
/*
 * Initializes the BFS source and destination based
 * on input, and sets the BFS graph to the graph object
 * that was sent.
 */


BFS::BFS(Graph* g) {
    graph = g;
}
/*
 * Prints the path from Source to Destination by
 * working backwards, from destination, and asking for the
 * node's 'previous' until arriving at the source.
 */
void BFS::PrintPath(Node* source, Node* destination) {
    Coordinate *path[100] = {};
    int count = 0;
    Node *currentNode = destination;
    Coordinate *c = ((*(currentNode)).getMyLocation());
    path[count] = c;
    count++;
    Node *n;
    do {
        n = ((*(graph)).getLocationOfPrev(currentNode));
        if (n->getMyLocation()->equalTo(source->getMyLocation())) {
            path[count] = n->getMyLocation();
            break;
        }
        path[count] = n->getMyLocation();
        count++;
        currentNode = n;
    } while (!(currentNode->getMyLocation()->equalTo(source->getMyLocation())));

    for (int i = count; i >= 0; i--) {
        Point point = path[i];
        cout << point << endl;


    }

    graph->deleteGraph();
}
/*
 * Gets the path from a source to a destination using a queue.
 * each iteration, it checks if it has arrived the destination.
 * Once it breaks, it prints the path.
 */
bool BFS::getPath() {
    Node* newSource;
    Coordinate *c1;
    Coordinate *c2;
    myDeque.push(source);
    newSource = source;
    do {
        /*
         * if visit neighbors returns a neighbor with -1 as
         * a coordinate, it means its an invalid path
         */
        vector<Node*> checkValid = visitNeighbors(newSource);
        if(checkValid.front()->getMyLocation()->getX() == -1) {
            //delete checkValid.front();
            return false;
        }
        /*
         * If the destination is included in the neighbors, we want to
         * return true, since we successfully arrived.
         */
        vector<Node *>::iterator v = checkValid.begin();
        while (v != checkValid.end()) {
            if((*(v))->getMyLocation()->equalTo(destination->getMyLocation())) {
                return true;
            }
            v++;
        }
        if (!myDeque.empty()) {
            myDeque.pop();
        }
        newSource = myDeque.front();
        if(newSource == NULL) {
            break;
        }
        c1 = (*(newSource)).getMyLocation();
        c2 = (*(destination)).getMyLocation();
    } while(!(c2->equalTo(c1)));
    return true;

}

/*
 * Function visits all the neighbors of the sent node, n, by calling
 * the node's function 'getNeighbors()'. It then iterates
 * over the vector of neighbors, to check if they've been visited.
 * If they haven't, its sets their member 'visited' to true and
 * pushes them onto the queue.
 */
std::vector<Node*> BFS::visitNeighbors(Node* n) {
    std::vector<Node *> neighbors = (*(graph)).getNeighbors(n);
    vector<Node *>::iterator v = neighbors.begin();
    /*
     * if getNeighbors returns a neighbor with -1 as
     * a coordinate, it means its an invalid path
     */
    if(neighbors.front()->getMyLocation()->getX() == -1) {
        return neighbors;
    }
    while (v != neighbors.end()) {
        if (!(*(*v)).isVisited()) {
            (*(*v)).visit();
            myDeque.push((*v));
        }
        v++;
    }
    return neighbors;
}

/*
 * This function first calls to the getPath function to set all the
 * previous Nodes, and then goes from
 * destination to source to get next place in path.
 */
vector<Coordinate*> BFS::getFullPath(Coordinate* sLoc, Coordinate* dLoc){
    vector<Coordinate*> path;

    vector<Coordinate*> tempPath;
    source =  graph->getNode(sLoc);
    this->destination = graph->getNode(dLoc);
    this->source->visit();
    bool valid = getPath();
    //If returned invalid, then no path exists
    if(!valid) {
        path.push_back(new Point(-1,-1));
        return path;
    }
    Node *node= destination;
    Node* previousNode;
    while (node != source && node->getPrev()!= NULL){
        tempPath.push_back(node->getMyLocation());
        previousNode = node;
        node = node->getPrev();
    }
    int i = 1;
    Coordinate* p = *(tempPath.end());
    while(i<= tempPath.size()){
        Coordinate* p = new Point(*(tempPath.end() - i));
        path.push_back(p);
        i++;
    }

    return path;
}

/*
 * The function returns a node pointer of the source
 */
Node* BFS::getSource() {
    return source;
}

/*
 * The function returns a node pointer of the destination
 */
Node* BFS::getDest() {
    return destination;
}
