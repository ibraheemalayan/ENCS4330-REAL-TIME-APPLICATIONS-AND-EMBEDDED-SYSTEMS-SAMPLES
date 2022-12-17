#include <stdio.h>
#include <stdlib.h>  
#include <pthread.h>  

void do_one_thing(int *);  
void do_another_thing(int *);  
void do_wrap_up(int, int);  

int r1 = 0, r2 = 0, r3 = 0;  

pthread_mutex_t r3_mutex=PTHREAD_MUTEX_INITIALIZER;  

extern int main(int argc, char **argv)  
{  
  pthread_t       thread1, thread2;  
  
  if ( argc != 2 ) {
    fprintf(stderr, "You must supply an integer as argument!\n");
    exit(-1);
  }
  
  r3 = atoi(argv[1]);  
  
  pthread_create(&thread1,  
		 NULL,  
		 (void *) do_one_thing,  
		 (void *) &r1);  
  
  pthread_create(&thread2,  
		 NULL,  
		 (void *) do_another_thing,  
		 (void *) &r2);  
  
  pthread_join(thread1, NULL);  
  pthread_join(thread2, NULL);  
  
  do_wrap_up(r1, r2);  
  
  return 0;  
} 

void do_one_thing(int *pnum_times)  
{  
  int i, j, k, x;  
  
  pthread_mutex_lock(&r3_mutex);  
  
  if ( r3 > 0 ) {  
    x = r3;
    r3++;  
  }
  else  
    x = 1;  
  
  pthread_mutex_unlock(&r3_mutex);  
  
  for ( i = 0;  i < 4; i++ ) {  
    printf("doing one thing\n");  
    for ( j = 0; j < 10000; j++ ) 
      x = x + i;  
    (*pnum_times)++;  
    
    for ( k = 0; k < 1000000; k++ );
  }  
} 

void do_another_thing(int *pnum_times)  
{  
  int i, j, k, x;  
  
  pthread_mutex_lock(&r3_mutex);  
  
  if ( r3 > 0 ) {  
    x = r3;  
    r3--;  
  }
  else
    x = 1;  
  
  pthread_mutex_unlock(&r3_mutex);  
  
  for ( i = 0;  i < 4; i++ ) {  
    printf("doing another \n");
    for ( j = 0; j < 10000; j++ ) 
      x = x + i;  
    
    (*pnum_times)++;  
    
    for ( k = 0; k < 1000000; k++ );
  }  
}  

void do_wrap_up(int one_times, int another_times)  
{  
  int total;  
  
  total = one_times + another_times;  
  printf("wrap up: one thing %d, another %d, total %d\n",  
	 one_times, another_times, total);  
  
  printf("r3 = %d\n", r3);
} 

 
