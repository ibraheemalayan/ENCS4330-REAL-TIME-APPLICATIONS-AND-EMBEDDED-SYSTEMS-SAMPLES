#include <stdio.h>       /* standard I/O routines                 */
#include <pthread.h>     /* pthread functions and data structures */

/* function to be executed by the new thread */
void*
do_loop(void* data)
{
  int i;			/* counter, to print numbers */
  int j;			/* counter, for delay        */
  int me = *((int*)data);     /* thread identifying number */
  
  for (i=0; i<10; i++) {
    //for (j=0; j<500000; j++) /* delay loop */
    // ;
    printf("'%d' - Got '%d'\n", me, i);
    //fflush(stdout);
  }
  
  /* exit the thread */
  if (me == 1) {
    pthread_exit((void *) -2);
  }
}

/* like any C program, program's execution begins in main */
int
main(int argc, char* argv[])
{
  int        thr_id;         /* thread ID for the newly created thread */
  pthread_t  p_thread;       /* thread's structure                     */
  int        a         = 1;  /* thread 1 identifying number            */
  int        b         = 2;  /* thread 2 identifying number            */
  int        status;
  
  /* run 'do_loop()' in the main thread as well */
  do_loop((void*)&b);

  /* create a new thread that will execute 'do_loop()' */
  thr_id = pthread_create(&p_thread, NULL, do_loop, (void*)&a);
    
  //pthread_join(p_thread, NULL);  
  pthread_join(p_thread, (void *) &status);  

  printf("thread ended with status %d\n", status);
  
  /* NOT REACHED */
  return 0;
}
