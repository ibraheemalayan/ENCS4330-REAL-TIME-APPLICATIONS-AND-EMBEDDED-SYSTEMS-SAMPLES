/*
 * Displaying message queue status information
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

main(void)
{
  int             mid;
  key_t           key;
  struct msqid_ds buf;
  
  key = ftok(".", 'z');
    
  if ( (mid = msgget(key, IPC_CREAT | 0777)) == -1 ) {
    perror("Queue create");
    exit(1);
  }
  
  msgctl(mid, IPC_STAT, &buf);
  
  printf("Message Queue *Permission* Structure Information\n");
  printf("Owner's user ID   \t%d\n", buf.msg_perm.uid);
  printf("Owner's group ID  \t%d\n", buf.msg_perm.gid);
  printf("Creator's user ID \t%d\n", buf.msg_perm.cuid);
  printf("Creator's group ID\t%d\n", buf.msg_perm.cgid);
  printf("Access Mode in HEX \t%06X\n", buf.msg_perm.mode);
  printf("\nAdditional Selected Message Queue Structure Information\n");
  printf("Current # of bytes on queue\t %d\n", buf.msg_cbytes);
  printf("Current # of messages on queue\t %d\n", buf.msg_qnum);
  printf("Maximum # of bytes on queue\t %d\n", buf.msg_qbytes);
  msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
  exit(0);
}
