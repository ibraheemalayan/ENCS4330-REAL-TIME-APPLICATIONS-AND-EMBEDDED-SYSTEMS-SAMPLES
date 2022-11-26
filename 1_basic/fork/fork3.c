/*
 * The fork() function
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  int i, status;
  pid_t pid, pid_array[3];
  
  printf("My process ID is %d\n", getpid());
  
  for ( i = 0; i < 3; i++ ) {
    pid = fork();
    
    if ( pid == -1)
      exit(-1);
    
    else if ( pid == 0 ) {
      printf("I am the child  => PID = %d\n", getpid());
      if (i != 1)
	while(1);
      else
	exit(-1);
    }
    else {
      printf("I am the parent => PID = %d, child ID = %d\n", getpid(), pid);
      pid_array[i] = pid;
    }
  }
  //while( 1 ) {
    for ( i = 0; i < 3; i++ ) {
      if (waitpid(pid_array[i], &status, 0) == pid_array[i]) {
	printf("Process ID %d has terminated\t status = %d\n", pid_array[i], status);
      }
    }
  //}
  
  return(0);
}
