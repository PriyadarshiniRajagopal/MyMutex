/*
 Single Author info:
 dvvira Denil Vijay Vira 
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */

#include "mythread.h"
#include "mymutex.h"

#ifndef MYCOND_H
#define MYCOND_H

typedef struct mythread_condattr
{
  void *args;                      // attributes of the condition variable, ignored in this program.
} mythread_condattr_t;

typedef struct mythread_cond
{
  mythread_queue_t queue;         // queue of threads waiting for signal on the condition variable
  int valid;                      // to check if the condition variable was destroyed. 
} mythread_cond_t;

int mythread_cond_init (mythread_cond_t *cond, mythread_condattr_t *cond_attr);

int mythread_cond_signal (mythread_cond_t *cond);

int mythread_cond_broadcast (mythread_cond_t *cond);

int mythread_cond_wait (mythread_cond_t *cond, mythread_mutex_t *mutex);

int mythread_cond_destroy (mythread_cond_t *cond);

#endif
