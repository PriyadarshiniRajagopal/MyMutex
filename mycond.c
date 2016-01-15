/*
 Single Author info:
 dvvira Denil Vijay Vira 
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */
#include "mythread.h"
#include "mycond.h"
#include "mymutex.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int mythread_cond_init (mythread_cond_t *cond, mythread_condattr_t *cond_attr)
{
  // if cond is NULL, return EINVAL
  if (!cond)
    return EINVAL;

  cond->queue = NULL;           // initialize the wait queue to NULL
  cond->valid = 1;              // Make the condition varialble valid
  return 0;                     // success
}

int mythread_cond_signal (mythread_cond_t *cond)
{
  // if cond is NULL, return EINVAL
  if (!cond)
    return EINVAL;
 
  // if the condtion variable was previously destroyed, return error code.
  if (cond->valid == -1)
    return EINVAL;  
     
  // if queue is not empty, unblock single thread from the waiting queue.  
  if (cond->queue)
  {      
    mythread_enter_kernel();
    mythread_unblock (&cond->queue, 0); 
  }
  
  // if queue is empty, do nothing and return
  return 0;  // success
}

int mythread_cond_broadcast (mythread_cond_t *cond)
{
  // if cond is NULL, return EINVAL
  if (!cond)
    return EINVAL;
     
  // if the condtion variable was previously destroyed, return error code.
  if (cond->valid == -1)
    return EINVAL;  

  // if queue is not empty, unblock all threads from the waiting queue.  
  while (cond->queue)
  {      
    mythread_enter_kernel();
    mythread_unblock (&cond->queue, 0); 
  }
  
  // if queue is empty, do nothing and return
  return 0;  // success
}

int mythread_cond_wait (mythread_cond_t *cond, mythread_mutex_t *mutex)
{

  // if either of condition variable or mutex is NULL, return EINVAL
  if (!cond || !mutex)
    return EINVAL;

  // if the condtion variable was previously destroyed, return error code.
  if (cond->valid == -1)
    return EINVAL;  

  // prepare to block, set the state of current thread to 1
    mythread_enter_kernel ();
    mythread_block_phase1 (&cond->queue, 1);

    // unlock the mutex before blocking current thread completely
    mythread_mutex_unlock (mutex);
 
    mythread_enter_kernel();

    // check if we received a signal in the meanwhile. If we did receive a signal, our thread state would be 0.
    if ((mythread_self())->state == 0)
    { 
      mythread_leave_kernel();
      // the current thread received the signal and no longer needs to block. Return. 
      mythread_mutex_lock (mutex);
      return 0;    
    }
  
    // if the signal was not received, then block the current thread. 
    mythread_block_phase2(); 
 
    // The current thread received signal and was woken up. Grab the lock and continue.    
    mythread_mutex_lock (mutex);

    return 0; // success
}


int mythread_cond_destroy (mythread_cond_t *cond)
{
/* if the condition variable argument does not exist, return error code */
  if (!cond)
    return EINVAL;

/* if there are threads still waiting on condition variable, return error */
  if (cond->queue)
    return EBUSY;

/* make the condition variable invalid if none of above two conditions apply */
  memset (cond, 0, sizeof(mythread_cond_t));
  cond->valid = -1;

  return 0; // success
}

