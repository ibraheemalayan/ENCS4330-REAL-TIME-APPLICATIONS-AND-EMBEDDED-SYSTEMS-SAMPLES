#ifndef __LOCAL_H_
#define __LOCAL_H_

/*
 * Common header file: parent, producer and consumer
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>

#define ROWS   5
#define COLS   3

#define SLOT_LEN  50
#define N_SLOTS   6

/* This declaration is *MISSING* is many solaris environments.
   It should be in the <sys/sem.h> file but often is not! If 
   you receive a duplicate definition error message for semun
   then comment out the union declaration.
   */

union semun {
  int              val;
  struct semid_ds *buf;
  ushort          *array; 
};

struct MEMORY {
  char buffer[N_SLOTS][SLOT_LEN];
  int  head, tail;
};

struct sembuf acquire = {0, -1, SEM_UNDO}, 
              release = {0,  1, SEM_UNDO};

enum {AVAIL_SLOTS, TO_CONSUME};

#endif

