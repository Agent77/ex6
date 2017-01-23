
#ifndef PRACTICE6_CLOCK_H
#define PRACTICE6_CLOCK_H
#include <boost/serialization/access.hpp>

/*
 * Class which represents the clock of the program.
 * It says the time whenever it is needed for the flow.
 */
class Clock {
private:
    int time;
public:
    Clock();
    int getTime();
    void increaseTime();

};


#endif //PRACTICE6_CLOCK_H
