#include <stdlib.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/shm.h>  
#include <sys/wait.h>  
 
#ifndef _POSIX_THREAD_PROCESS_SHARED  
#error "This platform does not support process shared mutex"  
#endif  
 
int   shared_mem_id;  
int   *shared_mem_ptr;  
pthread_mutex_t *mptr;  
pthread_mutex_attr_t mutex_shared_attr;  

extern int main(void)  
{  
  pid_t  child_pid;  
  int  status;  
  
  /* initialize shared memory segment */  
  shared_mem_id = shmget(IPC_PRIVATE, 1*sizeof(pthread_mutex_t), 0660);  
  shared_mem_ptr = (int *)shmat(shared_mem_id, (void *)0, 0);  
  mptr = shared_mem_ptr;  
  
  pthread_mutexattr_init(&mutex_shared_attr);  
  pthread_mutexattr_setshared(&mutex_shared_attr, PTHREAD_PROCESS_SHARED);  
  pthread_mutex_init(mptr, &mutex_shared_attr);  
  
  if ( (child_pid = fork()) == 0 ) {  
    
    /* child */  
    
    /* create more threads */  
    
    .  
 
    .  
 
    pthread_mutex_lock(mptr);  
 
    .  
 
    .  
 
   } else {  
 
     /* parent */  
     
     /* create more threads */  
 
     .  
 
     .  
 
     pthread_mutex_lock(mptr);  
 
     .  
     
    .  
} 
 
