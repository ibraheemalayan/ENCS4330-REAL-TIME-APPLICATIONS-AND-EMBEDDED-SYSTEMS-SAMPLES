/*
  SERVER-receives messages from clients
  */
#include "local.h"

int main(int argc, char *argv[ ]) {
  int     mid, n;
  MESSAGE msg;
  void    process_msg(char *, int);          
  
  if (argc != 3) {
    fprintf(stderr, "Usage: %s msq_id &\n", argv[0]);
    return 1;
  }
  
  mid = atoi(argv[1]);
  
  while (1) {
    //sleep(50);
    if ((n = msgrcv(mid, &msg, BUFSIZ, SERVER, 0)) == -1 ) {
      perror("Server: msgrcv");
      return 2;
    } 
    
    else if (n == 0) 
      break;               
    
    process_msg(msg.buffer, strlen(msg.buffer));
    msg.msg_to = msg.msg_fm;
    msg.msg_fm = SERVER;
    n += sizeof(msg.msg_fm);
    
    if (msgsnd(mid, &msg, n, 0) == -1 ) {   
      perror("Server: msgsnd");
      return 3;
    }
  }
  msgctl(mid, IPC_RMID, (struct msqid_ds *) 0 ); 
  exit(0);
}
/*
  Convert lowercase alphabetics to uppercase
*/
void
process_msg(char *b, int len)
{
  int i;
  
  for (i = 0; i < len; ++i)
    if (isalpha(*(b + i)))
      *(b + i) = toupper(*(b + i));
}
