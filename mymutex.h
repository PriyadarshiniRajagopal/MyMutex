/*
 Single Author info:
 prajago4 Priyadarshini Rajagopal
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */

#include "mythread.h"
#include "myatomic.h"
#include <string.h>

#ifndef MYMUTEX_H
#define MYMUTEX_H

#define RETRY_MAX 50              // maximum number of retries to acquire the lock before the thread blocks itself.
#define BLOCKED 1
#define RUNNING 0

typedef struct mythread_mutex
{
  int value;                      // value of lock, 0 means available, 1 means busy. 
  mythread_queue_t queue;         // queue of threads waiting to acquire the lock
  mythread_t thread;              // thread that currently owns the lock
} mythread_mutex_t;


typedef struct mythread_mutexattr
{
 void *data;                      // mutex attributes, ignored in this program
} mythread_mutexattr_t;

int mythread_mutex_init (mythread_mutex_t *mutex, mythread_mutexattr_t *attr);

int mythread_mutex_lock (mythread_mutex_t *mutex);

int mythread_mutex_unlock (mythread_mutex_t *mutex);

int mythread_mutex_destroy (mythread_mutex_t *mutex);

#endif
