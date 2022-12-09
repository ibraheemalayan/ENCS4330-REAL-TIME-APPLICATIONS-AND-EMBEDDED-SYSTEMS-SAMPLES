/*
 * The PRODUCER
 */

#include "local.h"

main(int argc, char *argv[])
{
  static char  *source[ROWS][COLS] = {
    {"A", "The", "One"},
    {" red", " polka-dot", " yellow"},
    {" spider", " dump truck", " tree"},
    {" broke", " ran", " fell"},
    {" down", " away", " out"}
  };
  
  static char    local_buffer[SLOT_LEN];
  int            i, r, c, sleep_limit, semid, shmid;
  pid_t          ppid = getppid();
  char          *shmptr;
  struct MEMORY *memptr;
  
  
  if ( argc != 2 ) {
    fprintf(stderr, "%s sleep_time\n", argv[0]);
    exit(-1);
  }
  
  /*
   * Access, attach and reference the shared memory
   */
  if ( (shmid = shmget((int) ppid, 0, 0)) != -1 ) {
    if ( (shmptr = (char *) shmat(shmid, (char *)0, 0)) 
	 == (char *) -1 ) {
      perror("shmat -- producer -- attach");
      exit(1);
    }
    memptr = (struct MEMORY *) shmptr;
  }
  else {
    perror("shmget -- producer -- access");
    exit(2);
  }
  
  /*
   * Access the semaphore set
   */
  if ( (semid = semget((int) ppid, 2, 0)) == -1 ) {
    perror("semget -- producer -- access");
    exit(3);
  }
  
  sleep_limit = atoi(argv[1]) % 20;
  
  i = 20 - sleep_limit;
  
  srand((unsigned) getpid());
  
  while ( i-- ) {
    memset(local_buffer, '\0', sizeof(local_buffer));
    
    for ( r = 0; r < ROWS; ++r ) { /* Make a random string */
      c = rand() % COLS;
      strcat(local_buffer, source[r][c]);
    }
    
    acquire.sem_num = AVAIL_SLOTS;
    
    if ( semop(semid, &acquire, 1) == -1 ) {
      perror("semop -- producer -- acquire");
      exit(4);
    }
    strcpy(memptr->buffer[memptr->tail], local_buffer);
    printf("P: [%d] %s.\n", memptr->tail, memptr->buffer[memptr->tail]);
    
    memptr->tail = (memptr->tail + 1) % N_SLOTS;

    release.sem_num = TO_CONSUME;
    
    if ( semop(semid, &release, 1) == -1 ) {
      perror("semop -- producer -- release");
      exit(5);
    }
    sleep(rand() % sleep_limit + 1);
  }
    
  exit(0);
}
