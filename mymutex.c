/*
 Single Author info:
 prajago4 Priyadarshini Rajagopal
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */

#include "mymutex.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* Initialize the mutex */
int mythread_mutex_init (mythread_mutex_t *mutex, mythread_mutexattr_t *attr)
{
  
  mutex->value = 0;     // initial value of mutex is zero
  mutex->queue = NULL;  // initializing the queue head with NULL

  return 0; // success
}

int mythread_mutex_lock (mythread_mutex_t *mutex)
{
  // if mutex is not poinitng to anything valid then return EINVAL
  if (!mutex)
    return EINVAL;

  if (mutex->value == -1)
    return EINVAL;


  int retry = 0;
  while (1)
  {
    mythread_enter_kernel();
    // check if lock is available for atmost 50 times
    while ((mutex->value == 1) && (retry < RETRY_MAX))
    {
       retry++;
    }

    // if we waited for 50 times and lock still not avaialable then break from he loop and block the current thread.
    if (retry == RETRY_MAX)
    {
      break;
    }
    
    // if lock is avaialable, grab it atomically using compare and swap function  
    if (compare_and_swap (&mutex->value, 1, 0)==0)
    {
      // update the current owner to mutex to current thread
      mutex->thread = mythread_self();
      mythread_leave_kernel();

      return 0; // success
    }    
  } // end of while loop
  
 // if lock is not available after 50 tries, block the current thread.
  mythread_block (&mutex->queue, BLOCKED);
  
  return 0; // success 
}

int mythread_mutex_unlock (mythread_mutex_t *mutex)
{
  // if mutex is not pointing to anything valid then return EINVAL
  if (!mutex)
    return EINVAL;

  if (mutex->value == -1)
    return EINVAL;

  mythread_enter_kernel ();

  // set the mutex value to zero to indicate that mutex is available.
  mutex->value = 0;


  // unblock the next thread from the mutex queue waiting for the lock. 
  mythread_unblock (&mutex->queue, RUNNING);
  
  return 0; // success
}

int mythread_mutex_destroy (mythread_mutex_t *mutex)
{
  // if not a valid mutex, return EINVAL
  if (!mutex)
    return EINVAL;
  
  // if mutex queue is not yet empty, means there are threads waiting to acquire the mutex, return EBUSY
  if (mutex->queue)
    return EBUSY;

  // make the mutex invalid by setting its value to -1.
  memset (mutex, 0, sizeof(mythread_mutex_t));
  mutex->value = -1;
  
  return 0; // success
}
