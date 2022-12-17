/*
 * Setting the scheduling policy and priority for a thread
 */

#include <stdio.h>  
#include <pthread.h> 
#include <sched.h>

void do_one_thing(int *);  
 
int r1 = 0;  

extern int main(void)  
{  
  pthread_attr_t custom_sched_attr; 		 
  int max_prio, min_prio, mid_prio; 
  struct sched_param param; 		 
  pthread_t thread1;
  
  
  pthread_attr_init(&custom_sched_attr);
  pthread_attr_setinheritsched(&custom_sched_attr, PTHREAD_EXPLICIT_SCHED); 		 
  pthread_attr_setschedpolicy(&custom_sched_attr, SCHED_FIFO); 		 
  
  max_prio = sched_get_priority_max(SCHED_FIFO); 		 
  min_prio = sched_get_priority_min(SCHED_FIFO); 		 
  
  mid_prio = (min_prio + max_prio)/2; 		 
  param.sched_priority = mid_prio; 		 
  
  pthread_attr_setschedparam(&custom_sched_attr, &param); 		 
  
  pthread_create(&thread1, 
		 &custom_sched_attr, 
		 (void *) do_one_thing,  
                 (void *) &r1); 	 

  return 0;  
}  

void do_one_thing(int *pnum_times)  
{  
  int i, j, x;  
  
  for ( i = 0;  i < 4; i++ ) {  
    printf("doing one thing\n");  
    
    for ( j = 0; j < 10000; j++ ) 
      x = x + i;  
    
    (*pnum_times)++;  
  }  
}  
