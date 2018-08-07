#include <pthread.h>
#include "binary_semaphore.h"

void semInitB(binary_semaphore* s, int state)
{
  pthread_mutex_init(&(s->mutex), NULL); // initialize mutex
  pthread_cond_init(&(s->cv), NULL);     // initialize condition variable
  s->flag = state;                       // set flag value
}

void semWaitB(binary_semaphore* s)
{
  // try to get exclusive access to s->flag
  pthread_mutex_lock(&(s->mutex));

  // no other thread can get here unless current thread unlocks "mutex"

  // examine the flag and wait until flag == 1
  while (s->flag == 0) {
    pthread_cond_wait(&(s->cv), &(s->mutex));
    
    // When the current thread executes the pthread_cond_wait()
    // statement, the current thread will be blocked on s->cv and
    // (atomically) unlocks s->mutex. Unlocking s->mutex will let
    // other threads in to test s->flag.
  }

  // -----------------------------------------
  // If the program arrives here, we know that s->flag == 1 and this
  // thread has now successfully passed the semaphore
  // -------------------------------------------
  s->flag = 0;  // This will cause all other threads that execute a
                // semWaitB() call to wait in the (above) while-loop

  // release exclusive access to s->flag
  pthread_mutex_unlock(&(s->mutex));  
}

void semSignalB(binary_semaphore* s)
{
  // try to get exclusive access to s->flag
  pthread_mutex_lock(&(s->mutex));

  pthread_cond_signal(&(s->cv));

  // This call may restart some thread that was blocked on s->cv (in
  // the semWaitB() call) if there was no thread blocked on cv, this
  // operation does nothing.

  // update semaphore state to Up
  s->flag = 1;

  // release exclusive access to s->flag
  pthread_mutex_unlock(&(s->mutex)); 
}
