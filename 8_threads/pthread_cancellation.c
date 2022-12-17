#include <stdlib.h>   
#include <stdio.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <pthread.h>   

#define NUM_THREADS 3   
#define MESSAGE_MAX_LEN 256

int count = NUM_THREADS;   
 
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;   
 
pthread_cond_t init_done=PTHREAD_COND_INITIALIZER;   
 
int id_arg[NUM_THREADS] = {0,1,2};   
 
extern int   
 
main(void)   
 
{   
  int i;   
  void *statusp;
  void *ask_for_it(int *my_id);  
  void *sitting_duck(int *my_id);
  void *bullet_proof(int *my_id);
  
  pthread_t threads[NUM_THREADS];   
 
  
  /**** Create the threads ****/   
  pthread_create(&(threads[0]), NULL, ask_for_it, (void *) &(id_arg[0]));   
  pthread_create(&(threads[1]), NULL, sitting_duck, (void *) &(id_arg[1]));   
  pthread_create(&(threads[2]), NULL, bullet_proof, (void *) &(id_arg[2]));   
  
  printf("main(): %d threads created\n",count);   
 
  /**** wait until all threads have initialized ****/   
  pthread_mutex_lock(&lock);   
 
  while (count != 0) {
    pthread_cond_wait(&init_done, &lock);   
  }   
  
  pthread_mutex_unlock(&lock);   
  
  printf("main(): all threads have signaled that they're ready\n");   
  
  /**** cancel each thread ****/   
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_cancel(threads[i]);   
  }   
  
  /**** wait until all threads have finished ****/   
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], &statusp);   
 
    if (statusp == PTHREAD_CANCELED) {   
      printf("main(): joined to thread %d, statusp=PTHREAD_CANCELED\n", i);   
      
    } 
    else {
      printf("main(): joined to thread %d \n", i);   
    }   
  }
 
  printf("main(): all %d threads have finished. \n", NUM_THREADS);   
  
  return 0;   
}  
 
void *bullet_proof(int *my_id)   
{
  
  int i=0, last_state;   
  char *messagep;   
 
  messagep = (char *)malloc(MESSAGE_MAX_LEN);   
  
  sprintf(messagep, "bullet_proof, thread #%d: ", *my_id);   
  printf("%s\tI'm alive, setting general cancelability OFF\n", messagep);   
  
  /* We turn off general cancelability here */   
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &last_state);   
  pthread_mutex_lock(&lock);   
 
  {   
    printf("\n%s signaling main that my init is done\n", messagep);   
    
    count -= 1;   
    
    /* Signal to program that loop is being entered */   
    pthread_cond_signal(&init_done);   
    pthread_mutex_unlock(&lock);   
  }   
  
  /* Loop forever until picked off with a cancel */   
  for(;;i++) {
    if (i%10000 == 0)   
      print_count(messagep, *my_id, i);   
    
    if (i%100000 == 0)   
      printf("\n%s This is the thread that never ends... #%d\n", messagep, i);   
  }   
  
  /* Never get this far */   
  return(NULL);   
}  
 
void *ask_for_it(int *my_id)   
{
  int i=0, last_state, last_type;   
  char *messagep;   
  
  messagep = (char *)malloc(MESSAGE_MAX_LEN);   
  
  sprintf(messagep, "ask_for_it, thread #%d: ", *my_id);   
  
  /* We can turn on general cancelability here and disable async cancellation. */   
  printf("%s\tI'm alive, setting deferred cancellation ON\n", messagep);   
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_state);   
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &last_type);   
  
  pthread_mutex_lock(&lock);   
  {
    printf("\n%s signaling main that my init is done\n", messagep);   
    
    count -= 1;   
    
    /* Signal to program that loop is being entered */   
    pthread_cond_signal(&init_done);   
    pthread_mutex_unlock(&lock);   
  }   
  
  /* Loop forever until picked off with a cancel */   
  for(;;i++) {
    if (i%1000 == 0)   
      print_count(messagep, *my_id, i);   
    
    if (i%10000 == 0)   
      printf("\n%s\tLook, I'll tell you when you can cancel me.%d\n", messagep, i);   
 
    pthread_testcancel();   
  }   
  
  /* Never get this far */   
  return(NULL);   
}  
 
void *sitting_duck(int *my_id)   
{
  int i=0, last_state, last_type, last_tmp;   
  char *messagep;   
  
  messagep = (char *)malloc(MESSAGE_MAX_LEN);   
  sprintf(messagep, "sitting_duck, thread #%d: ", *my_id);   
  
  pthread_mutex_lock(&lock);   
  {
    printf("\n%s signaling main that my init is done\n", messagep);   
    
    count -= 1;   
    
    /* Signal to program that loop is being entered */   
    pthread_cond_signal(&init_done);   
    pthread_mutex_unlock(&lock);   
  }   
   
  /* Now, we're safe to turn on async cancelability */   
  printf("%s\tI'm alive, setting async cancellation ON\n", messagep);   
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &last_type);   
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_state);   
  
  /* Loop forever until picked off with a cancel */   
  for(;;i++) {
    if ((i%1000) == 0)   
      print_count(messagep, *my_id, i);   
    
    if (i%10000 == 0) {
      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &last_tmp);   
      printf("\n%s\tHum, nobody here but us chickens. %d\n", messagep, i);   
      pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &last_tmp);   
    }   
  }   
  
  /* Never get this far */   
  return(NULL);   
}  

