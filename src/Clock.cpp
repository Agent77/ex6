//
// Created by nirbs on 27/12/16.
//

#include "Clock.h"
/*
 * Constructor initializes the time to 0
 */
Clock::Clock(){
    time=0;
}

/*
 * The function returns the current time
 */
int Clock::getTime() {
    return time;
}

/*
 * The function increases the time by 1
 */
void Clock::increaseTime() {
    time++;
}