/*
 * The fork() function
 */
#include <stdio.h>

int main()
{
  int i, pid;
  
  printf("My process ID is %d\n", getpid());

  for ( i = 0; i < 3; i++ ) {
    pid = fork();
    
    if ( pid == 0 ) {
      printf("I am the child  => PID = %d\n", getpid());
      while(1);
    }
    else
      printf("I am the parent => PID = %d, child ID = %d\n", getpid(), pid);
  }
  while(1);
  return(0);
}
