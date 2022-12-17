#include <stdio.h>  
#include <pthread.h>  
 
pthread_t thread;  
static int arg;  
static const int internal_error = -12;  
static const int normal_error = -10;  
static const int success = 1;  

void * routine_x(void *arg_in)  
{  
  int *arg = (int *)arg_in;  
  
  .  
    
  .  
    
  .  
    
  if ( /* something that shouldn't have happened */) {  
    pthread_exit((void *) &internal_error);  
 
  }
  else if ( /* normal failure */ ) {  
    return ((void *) &normal_error);  
 
    }
  else {  
    return ((void *) &success);  
  }  
}  
 
extern int main(int argc, char **argv)  
{  
  pthread_t thread;  
  void *statusp;  
 
    .  
 
    .  
 
    .  
 
    pthread_create(&thread, NULL, routine_x, &arg);  
    pthread_join(thread, &statusp);  
 
    if ( *statusp == PTHREAD_CANCELED ) {  
      printf("Thread was canceled.\n");  
    }
    else {  
      printf("Thread completed and exit status is %ld.\n", *(int *)statusp);  
    }  
 
    return 0;  
} 
 
