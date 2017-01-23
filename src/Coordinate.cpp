#include "Coordinate.h"

/*
 * allows any function to request an objects location,
 * without having to know how many coordinates it contains
 * (ie, which dimension it is in)
 */
int Coordinate::getNextCoordinate(int place) {
    return locs[place];
}
/*
 * Copies the coordinates from one point to another
 */
Coordinate::Coordinate(Coordinate* p) {
    int size = (int)sizeof( p->getCoordinates()) / 4;
    for (int i = 0; i < size; i++) {
        locs[i] = p->locs[i];
    }
}

/*
 * An overload function for print
 */
std:: ostream& operator<<(std :: ostream& out, Coordinate& t) {
    t.printFormat(out);
    return out;
}



/*
 * Returns a generic Coordinate with the locations of
 * the Point.
 */
int* Coordinate::getCoordinates() {
    return locs;
}
