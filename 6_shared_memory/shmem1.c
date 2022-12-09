/*
 * Allocating a shared memory segment
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void)
{
  key_t    key = 15;
  int      shmid_1, shmid_2;
  static char message[BUFSIZ];
  
  if ( (shmid_1 = shmget(key, 1024, 0644 | IPC_CREAT)) == -1 ) {
    perror("shmget: shmid_1");
    exit(1);
  }
  
  printf("First shared memory identifier is %d\n", shmid_1);

  if ( (shmid_2 = shmget(IPC_PRIVATE, 20, 0644)) == -1 ) {
    perror("shmget: shmid_2");
    exit(2);
  }
  
  printf("Second shared memory identifier is %d\n", shmid_2);
  
  return(0);
}
