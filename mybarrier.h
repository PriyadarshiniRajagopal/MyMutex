/*
 Single Author info:
 wli29 Wanqui Li
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */
#include "mythread.h"

#ifndef MYBARRIER_H
#define MYBARRIER_H

#define MYTHREAD_BARRIER_SERIAL_THREAD 1

typedef struct mythread_barrier_t {
    mythread_queue_t queue;           // queue of waiting threads
    int count;                        // count of number of threads left to call wait. 
    int init_count;                   // total number of threads calling wait. 
} mythread_barrier_t;

typedef struct mythread_barrierattr_t {
    /* Not implemented. */
    char c;
} mythread_barrierattr_t;

int mythread_barrier_init (mythread_barrier_t*, const mythread_barrierattr_t*, unsigned);
int mythread_barrier_destroy (mythread_barrier_t*);
int mythread_barrier_wait (mythread_barrier_t*);

#endif
