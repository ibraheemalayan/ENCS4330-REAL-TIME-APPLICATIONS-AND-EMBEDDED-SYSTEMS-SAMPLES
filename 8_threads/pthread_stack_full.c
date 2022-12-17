#include <stdio.h>  
#include <pthread.h>  
 
void do_one_thing(int *);  
void do_another_thing(int *);  
void do_wrap_up(int, int);  

int r1 = 0, r2 = 0;  

pthread_attr_t stack_size_custom_attr; 

size_t default_stack_size;   

extern int main(void)  
{  
  pthread_t       thread1, thread2;  

  pthread_attr_getstacksize(&stack_size_custom_attr,   
			    &default_stack_size);   

  printf("Default Stack Size = %d\n", default_stack_size);
  
  exit(0);
  
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
  int i, j, x;  
  
  for ( i = 0;  i < 4; i++ ) {  
    printf("doing one thing\n");  
    
    //for ( j = 0; j < 1000000; j++ ) 
    //  x = x + i;  
    
    (*pnum_times)++;  
  }  
}  

void do_another_thing(int *pnum_times)  
{  
  int i, j, x;  
  
  for ( i = 0;  i < 4; i++ ) {  
    printf("doing another \n");  
    
    // for ( j = 0; j < 1000000; j++ ) 
    //x = x + i;  
    
    (*pnum_times)++;  
  }  
}  

void do_wrap_up(int one_times, int another_times)  
{  
  int total;  
  
  total = one_times + another_times;  
  printf("wrap up: one thing %d, another %d, total %d\n",  
	 one_times, another_times, total);  
}
