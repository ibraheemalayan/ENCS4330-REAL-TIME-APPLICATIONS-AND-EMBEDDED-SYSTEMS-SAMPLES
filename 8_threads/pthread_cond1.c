#include <stdio.h>
#include <unistd.h>  
#include <pthread.h>  

#define TCOUNT 25  
#define WATCH_COUNT 30  

int count = 0;  

void inc_count(int *);  
void watch_count(int *);  

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t count_threshold_cv = PTHREAD_COND_INITIALIZER;  
 
int  thread_ids[3] = {0,1,2};  
int r = 0;

int main(void)  
{  
  int      i;  
  pthread_t threads[3];  
 
  pthread_create(&threads[0],NULL, (void *) inc_count, &thread_ids[0]);  
  pthread_create(&threads[1],NULL, (void *) inc_count, &thread_ids[1]);  
  pthread_create(&threads[2],NULL, (void *) watch_count, &thread_ids[2]);  
   
  for ( i = 0; i < 3; i++ )
    pthread_join(threads[i], NULL);  
  
  printf("r = %d\n", r);
  
  return 0;  
}  

void watch_count(int *idp)  
{  
  pthread_mutex_lock(&count_mutex);
    
  if (count <= WATCH_COUNT) {  
    pthread_cond_wait(&count_threshold_cv, &count_mutex);  
    printf("watch_count(): Thread %d,Count is %d\n", *idp, count);
    r = 1;
  }  
  pthread_mutex_unlock(&count_mutex);  
}  

void inc_count(int *idp)  
{  
  int i;
  
  for ( i = 0; i < TCOUNT; i++ ) {  
    pthread_mutex_lock(&count_mutex);  
    count++;  
    printf("inc_count(): Thread %d, old count %d,new count %d\n", 
	   *idp, count - 1, count );  
    
    if (count == WATCH_COUNT)  
      //pthread_cond_signal(&count_threshold_cv);  
      pthread_cond_broadcast(&count_threshold_cv);  

    pthread_mutex_unlock(&count_mutex);  

    //sleep(1);
  }  
} 
 

