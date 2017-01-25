

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Trip.h"
#include <queue>
#include <pthread.h>
using namespace std;


/***********************************************************************
* Class name: ThreadPool									   *									               *
* The class description: contains an array of threads
 * which *
 **********************************************************************/
class ThreadPool {
private:
    int threads_num;
    pthread_t* calculatorThreads;
    pthread_mutex_t lock;
    int currentId;
    bool stop;

public:
    ThreadPool(int threads_num);
    ThreadPool() {
    }
    /***********************************************************************
	* function name: startCalculations									   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: thread enters into a loop where it
     * checks if there are more trips to calculate in the queue.           *
	***********************************************************************/
    void startCalculations(int id);
    /***********************************************************************
	* function name: addTripToCalculate									   *
	* The Input: Trip													   *
	* The output: none										               *
	* The Function operation: adds a trip to the deque                     *
	***********************************************************************/
    void addTripToCalculate(Trip *t);
    void terminate();
    bool isEmpty();
    /***********************************************************************
	* function name: getThread                 								   *
	* The Input: int													   *
	* The output: none										               *
	* The Function operation: returns the thread which
     * is currently calculating the trip with the given id                  *
	***********************************************************************/
    pthread_t* getThread(int id);
    int getCurrentId();
    virtual ~ThreadPool();
};

#endif /* THREADPOOL_H_ */
