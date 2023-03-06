#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>


#define SEED 'W'
#define PERSON_ARGN 3
#define SHM_SIZE  4
#define Q_SIZE 10
#define TELLERN 10
#define MINUTE 1000000 // simulation minute is MINUTE useconds in real life

#define UNHAPPY 10
#define UNSERVED 50
#define HAPPY 10

enum stage{outside, queue, detector, waiting_area, office};
enum purpose {birth, reunion, travel ,ID};
enum gender {male, female};

static struct sembuf get = {0,-1,SEM_UNDO}, release = {0,1,SEM_UNDO}, try_get = {0,-1,IPC_NOWAIT}, officer_open = {0,Q_SIZE,SEM_UNDO};

static struct sembuf Fget = {1,-1,SEM_UNDO}, Frelease = {1,1,SEM_UNDO}, Ftry_get = {1,-1,IPC_NOWAIT};


typedef struct {
  pid_t      msg_fm;		/* Placed in the queue by  */
  long mesg_type;
} MESSAGE;

typedef struct{
  int shmid;
  int *shm;
}shminfo;

union semun{
 int val;
 struct semid_ds *buf;
 ushort *array;
};

shminfo createSHM(key_t key){
    shminfo shm;
    if ( (shm.shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0 ) {
        perror("shmget fail");
        exit(1);
    }
    if ( (shm.shm = (int *) shmat(shm.shmid, 0, 0)) == (int *) -1 ) {
        perror("shmat: parent");
        exit(2);
    }  
    return shm;
}

int createSEM(key_t key){
    int semid;
    union semun arg;
    if ((semid = semget(key,2,IPC_CREAT|IPC_EXCL|0660))!=-1){
        arg.val = 0;
        if (semctl(semid,0,SETVAL,arg) == -1){
            perror("semaphore setvalue error");
            exit(3);
        }
        
    }else{
        perror("can't even create the semaphore");
        exit(4);
    }
    return semid;
}

int createMSGQ(key_t key){
    int msgid;
    if ((msgid = msgget(key, 0666 | IPC_CREAT)) == -1){
        perror("message queue creation failed");
        exit(5);
    }
    return msgid;
}