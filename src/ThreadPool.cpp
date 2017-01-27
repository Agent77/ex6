
#include "ThreadPool.h"
queue<Trip *> tripsToCalculate;
#include <unistd.h>
#include <iostream>
static void *startThread(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    int id = pool->getCurrentId();
    pool->startCalculations(id);
    return NULL;
}

void ThreadPool::startCalculations(int sentId) {
    int id = sentId;
    sleep(1);
    while (!stop) {
        pthread_mutex_lock(&lock);
        if (!tripsToCalculate.empty()) {
            Trip* t = tripsToCalculate.front();
            tripsToCalculate.pop();
            t->setThreadId(id);
            pthread_mutex_unlock(&lock);
            bool valid = t->calculatePath();
            if(!valid) {
                t->setInvalid();
            }
            t->isCalculated(1);
        }
        else {
            pthread_mutex_unlock(&lock);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

void ThreadPool::addTripToCalculate(Trip *t) {
    tripsToCalculate.push(t);
}

ThreadPool::ThreadPool(int threads_num) : threads_num(threads_num), stop(false) {
    calculatorThreads = new pthread_t[threads_num];

    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < threads_num; i++) {
        sleep(1);
        currentId = i;
        pthread_create(calculatorThreads + i, NULL, startThread, this);
    }
}

void ThreadPool::terminate() {
    stop = true;
}

ThreadPool::~ThreadPool() {
    // TODO ADDED TERMINATE MAY NEED TO remove
    terminate();
    delete[] calculatorThreads;
    pthread_mutex_destroy(&lock);
}

bool ThreadPool::isEmpty() {

}

pthread_t* ThreadPool::getThread(int id) {
    return &calculatorThreads[id];
}

int ThreadPool::getCurrentId() {
    return currentId;
}