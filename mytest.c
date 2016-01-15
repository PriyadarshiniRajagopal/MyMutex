/*
 Single Author info:
 dvvira Denil Vijay Vira 
 
 Group info:
 dvvira Denil Vijay Vira 
 prajago4 Priyadarshini Rajagopal
 wli29 Wanqui Li
 */

#include "mycond.h"
#include "mybarrier.h"
#include "mymutex.h"
#include <stdio.h>

// set PASS to zero. For an error returned by any function, decrement PASS. 
int PASS = 0; 

// decalre mutex, condition variable and barrier
mythread_mutex_t mut; 
mythread_cond_t con; 
mythread_barrier_t barr;

// declare 6 threads, to be used in this test. 
mythread_t t1, t2, t3, t4, t5, t6;
int count = 1;
 
// grab the lock. If count is less than 100, then wait for signal. Once signal is recieved, unlock mutex and wait on barrier. 
void *foo1 (void *arg)
{
    int test;
    mythread_mutex_lock (&mut);
    
    while (count < 100)             
    {
      mythread_cond_wait (&con, &mut);  
    }
        
   if ( mythread_mutex_unlock (&mut) != 0)
     PASS--;
        
   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 
    PASS--;

    return NULL;
}
 
// grab the mutex, increment the count to 100. Send signal to waiting threads. Unlock the mutex and wait on barrier. 
void *foo2 (void *arg)
{
    int test;
    mythread_mutex_lock (&mut);
    while (count < 100)
    {
      count++;     
    }

   if ( mythread_cond_broadcast (&con) != 0)
     PASS--;
    
   if ( mythread_mutex_unlock (&mut) != 0)
     PASS--;
        
   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 

     PASS--;

    return NULL;
}

// count up to ceratin number and then wait on barrier. 
void *foo3 (void *arg)
{
  int test;
  int val = 0;
 
  while (val < 999)
    val++;

   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 
 
    PASS--;
 
  while (val > 0)
    val--;
  
  return NULL; 
}

// count up to ceratin number and then wait on barrier. 

void *foo4 (void *arg)
{
  int test;
  int val = 0;
 
  while (val < 9999)
    val++;

   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 
  
    PASS--;
 
  while (val > 0)
    val--;
  
  return NULL; 
}

// count up to ceratin number and then wait on barrier. 

void *foo5 (void *arg)
{
  int test;
  int val = 0;
 
  while (val < 99)
    val++;

   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 
   
    PASS--;
 
  while (val > 0)
    val--;
  
  return NULL; 
}


int main (void)
{
    int test;
    // initialize mutex, condition variable and barrier.   
    mythread_mutex_init (&mut, NULL);
    mythread_cond_init (&con, NULL);
    // 4 threads will be calling wait. 
    mythread_barrier_init (&barr, NULL, 4);

    mythread_setconcurrency (8);

    // create first 3 threads and assign them functions. 
    mythread_create (&t1, NULL, foo1, NULL);
    mythread_create (&t2, NULL, foo2, NULL);
    mythread_create (&t3, NULL, foo3, NULL);
    
    // stall main until all threads have finished. 
   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 
    
      PASS--;

   // after this stage, mutex and condition variables are not required, destroy them. 
   if ( mythread_cond_destroy (&con) != 0)
     PASS --;

   if ( mythread_mutex_destroy (&mut) != 0)
     PASS --;
  
    // create next 3 threads and assign functions. This is to test reusability of barriers. 
    mythread_create (&t4, NULL, foo3, NULL);
    mythread_create (&t5, NULL, foo4, NULL);
    mythread_create (&t6, NULL, foo5, NULL);
    
    // let main also wait on the barrier
   if ((test = mythread_barrier_wait (&barr) != 0) && (test != MYTHREAD_BARRIER_SERIAL_THREAD)) 
     PASS--;
 
    // at this point, we are sure that all the threads have finished running. Now check value of PASS for any errors. 
    // If PASS is 0, means all function executed successfully. If PASS is less than 0, atleast one function returned error. 
    // print PASS or FAIL accordingly. 

    if (PASS == 0)
      printf ("PASS\n");
    else
      printf ("FAIL\n");

    //destroy the barrier and exit. 
    mythread_barrier_destroy (&barr);      
    mythread_exit(NULL);
    return 0;
}

