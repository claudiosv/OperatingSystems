#ifndef binary_semaphore_impl_h
#define binary_semaphore_impl_h

#include <pthread.h>

typedef struct {
  pthread_cond_t  cv;    // cond. variable - used to block threads
  pthread_mutex_t mutex; // mutex variable - used to prevents concurrent
			 //   access to the variable "flag"
  int             flag;  // semaphore state:  0 = down, 1 = up
} binary_semaphore;

void semInitB  (binary_semaphore* s, int state);
void semWaitB  (binary_semaphore* s);
void semSignalB(binary_semaphore* s);

#endif // binary_semaphore_impl_h
