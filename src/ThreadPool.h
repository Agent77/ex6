

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Trip.h"
#include <queue>
#include <pthread.h>
using namespace std;



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
    void startCalculations(int id);
    void addTripToCalculate(Trip *t);
    void terminate();
    bool isEmpty();
    pthread_t* getThread(int id);
    int getCurrentId();
    virtual ~ThreadPool();
};

#endif /* THREADPOOL_H_ */
