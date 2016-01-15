/*
 Single Author info:
 wli29 Wanqui Li
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */
#include "mybarrier.h"
#include "mythread.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int mythread_barrier_init (mythread_barrier_t* barrier, const mythread_barrierattr_t* attr, unsigned count){

    // if barrier does not exisit, return EINVAL
    if (barrier == NULL)
        return EINVAL;
   
    barrier->count = count;        // initialize count of threads calling wait. 
    barrier->queue = NULL;         // initialize wait queue with NULL
    barrier->init_count = count;   // intialize with number of threads calling wait. 
    return 0; // success
}


int mythread_barrier_wait (mythread_barrier_t *barrier)
{

  // if barrier does not exist or if it was previously destroyed, then return error code.
  if (!barrier)
    return EINVAL;
  if (barrier->init_count == -1)
    return EINVAL;

  int i;
  mythread_enter_kernel();

  // A thread called wait, decrement the counter by 1. 
  if (barrier->count > 0)
    barrier->count--;

  // if this is the last thread calling the wait
  if (barrier->count == 0)
  {
    // reinitialize the barrier count before unblocking rest of threads, so that the barrier can be reused
    barrier->count = barrier->init_count;

    // there should exactly be (init_count - 1) number of threads in the queue at this point. Unblock all of them.  
    for (i = 0; i < barrier->init_count -1; i++)
    {
      mythread_unblock (&barrier->queue, 0);
      mythread_enter_kernel ();
    }

    mythread_leave_kernel();
    
    // only the last thread returns MYTHREAD_BARRIER_SERIAL_THREAD while rest of threads return 0. 
    return MYTHREAD_BARRIER_SERIAL_THREAD; // success
  }

 // if this is not the last thread calling wait, block the current thread. 
  else 
  mythread_block (&barrier->queue, 1);
  
  return 0; // success
}

int mythread_barrier_destroy (mythread_barrier_t* barrier){
    // if barrier does not exist, return error code  
    if (!barrier)
        return EINVAL;
   
    // if barrier queue is not empty, do not destroy barrier. return error code. 
    if (barrier->queue)
      return EBUSY;

    memset (barrier, 0, sizeof(mythread_barrier_t));
    barrier->init_count = -1;

    return 0; // success     
}
