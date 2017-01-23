#include "Grid.h"
#include <boost/serialization/export.hpp>

/*
 * The constructor initializes the grid with new Nodes,
 * which contain a Point of their location within
 * the grid.
 */
Grid::Grid(int xSize, int ySize) {
    sizeX = xSize;
    sizeY = ySize;
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            Point p = Point(i, j);
            //cout << "POINT: ("<<p.getX()<<","<<p.getY()<<")"<<endl;
            Node* n;
            n = new Node(&p);
            arrayOfPtrsToNodes[i][j] = n;
        }
    }
}
 Grid::Grid(Graph *g) {
    this->sizeX = g->getSizeX();
    this->sizeY = g->getSizeY();
    //Node** oldNodes = g->getNodes();
    int i;
    int j;
    for (i = 0; i < sizeX; i++) {
        for (j = 0; j < sizeY; j++) {
            Coordinate *c= new Point(i,j);
            Node* n;
            n = new Node(g->getNode(c));
            delete c;
            this->arrayOfPtrsToNodes[i][j] = n;
        }
    }
}

/*
 * function returns a pointer to the previous Node, from which
 * we arrived at our current Node.
 */
Node* Grid::getLocationOfPrev(Node* n) {
    Node* prev = n->getPrev();
    return prev;
}

/*
 * The function used the grid the get pointers to each
 * of the node's neighbors. Once it finds them,
 * if it hasn't been visited yet, it 'visits' them, and
 * sets their previous to be the node which was sent.
 * It returns them in a vector. If a node is a obstacle,
 * it is not even considered a possible neighbor.
 */
std::vector<Node*> Grid::getNeighbors(Node* n) {
    //Vector to return with neighbors inside
    std::vector<Node*> neighbors;
    int* p = (*(n)).getLocation();
    //Copy constructor to convert the generic Coordinate to type Point
    Point point(p[0], p[1]);
    int x = (point.getX());
    int y = (point.getY());
    //Left border of grid
    if ((x - 1) >= 0) {
        //9 o'clock neighbor
        Node* node = (arrayOfPtrsToNodes[(point.getX()) - 1][(point).getY()]);
        if(!(node->isVisited())){
            if(!node->isObstacle()) {
                node->setPrev(*n);
                neighbors.push_back(node);
            }
        }
    }
    //Top border of grid
    if (y + 1 < sizeY) {
        //12 o'clock neighbor
        Node* node = (arrayOfPtrsToNodes[(point).getX()][(point).getY() + 1]);
        if(!(node->isVisited())){
            if(!node->isObstacle()) {
                node->setPrev(*n);
                neighbors.push_back(node);
            }
        }
    }
    //Right border of grid
    if((x + 1) < sizeX) {
        //3 o'clock neighbor
        Node* node = arrayOfPtrsToNodes[(point).getX() + 1][(point).getY()];
        if(!(node->isVisited())){
            if(!node->isObstacle()) {
                node->setPrev(*n);
                neighbors.push_back(node);
            }
        }
    }
    //Bottom border of grid
    if((y - 1) >= 0) {
        //6 o'clock neighbor
        Node* node = arrayOfPtrsToNodes[point.getX()][point.getY() - 1];
        if(!(node->isVisited())){
            if(!node->isObstacle()) {
                node->setPrev(*n);
                neighbors.push_back(node);
            }
        }
    }
    return neighbors;
}
/*
 * Returns the appropriate Node in the same location in the
 * grid as the Node's location.
 */
Node* Grid::getNode(Coordinate* p){
    Node* node = (arrayOfPtrsToNodes[p->getNextCoordinate(0)][p->getNextCoordinate(1)]);
    return node;
}

/*
 * destructor for Grid.
 */
Grid::~Grid() {
}

/*
 * Goes through each node which was created with 'new' command
 * and deletes their Location Point and themselves.
 */
void Grid::deleteGraph() {
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            Node* n = arrayOfPtrsToNodes[i][j];
            delete n->getMyLocation();
            delete n;
        }
    }
}
/*
 * The function sets a given node to be visited so theres an obstacle in there
 */
void Grid::addObstacle(Coordinate* p) {
    Node* node = arrayOfPtrsToNodes[p->getNextCoordinate(0)][p->getNextCoordinate(1)];
    node->makeObstacle();
    delete p;
}

/*
*  resets the nodes so the same graph with the
* same obstacles can be used, without their previous
* assignments affecting the algorithm.
*/
void Grid::resetGraph(){
    for (int i=0; i<sizeX;i++){
        for (int j=0;j<sizeY;j++){
            arrayOfPtrsToNodes[i][j]->resetNode();
        }
    }
}
//void Grid::getNodes() {
  //  Node* n = arrayOfPtrsToNodes[2][2];
    //int num = n->getLocation()[0];
//}

int Grid::getSizeX() {
    return sizeX;
}

int Grid::getSizeY() {
    return sizeY;
}

