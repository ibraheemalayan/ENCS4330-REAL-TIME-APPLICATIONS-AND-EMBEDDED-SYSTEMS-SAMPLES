/*
 * The PARENT
 */

#include "local.h"

main(int argc, char *argv[])
{
  static struct  MEMORY memory;
  static ushort  start_val[2] = {N_SLOTS, 0};
  int            semid, shmid, croaker;
  char          *shmptr;
  pid_t          p_id, c_id, pid = getpid();
  union semun    arg;
  
  memory.head = memory.tail = 0;
  
  if ( argc != 3 ) {
    fprintf(stderr, "%s producer_time consumer_time\n", argv[0]);
    exit(-1);
  }

  /*
   * Create, attach and initialize the memory segment
   */
  if ( (shmid = shmget((int) pid, sizeof(memory),
		       IPC_CREAT | 0600)) != -1 ) {
    
    if ( (shmptr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
      perror("shmptr -- parent -- attach");
      exit(1);
    }
    memcpy(shmptr, (char *) &memory, sizeof(memory));
  }
  else {
    perror("shmid -- parent -- creation");
    exit(2);
  }
  
  /*
   * Create and initialize the semaphores
   */
  if ( (semid = semget((int) pid, 2,
		       IPC_CREAT | 0666)) != -1 ) {
    arg.array = start_val;
    
    if ( semctl(semid, 0, SETALL, arg) == -1 ) {
      perror("semctl -- parent -- initialization");
      exit(3);
    }
  }
  else {
    perror("semget -- parent -- creation");
    exit(4);
  }
  
  /*
   * Fork the producer process
   */
  if ( (p_id = fork()) == -1 ) {
    perror("fork -- producer");
    exit(5); 
  }
  else if ( p_id == 0 ) {
    execl("./producer", "./producer", argv[1], (char *) 0);
    perror("execl -- producer");
    exit(6);
  }
  
  /*
   * Fork the consumer process
   */
  if ( (c_id = fork()) == -1 ) {
    perror("fork -- consumer");
    exit(7); 
  }
  else if ( c_id == 0 ) {
    execl("./consumer", "./consumer", argv[2], (char *) 0);
    perror("execl -- consumer");
    exit(8);
  }
  
  croaker = (int) wait((int *) 0); /* wait for 1 to die */
  kill( (croaker == p_id) ? c_id : p_id, SIGKILL);
  
  shmdt(shmptr);
  shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0); /* remove */
  semctl(semid, 0, IPC_RMID, 0);
  
  exit(0);
}
