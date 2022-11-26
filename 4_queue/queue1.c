/*
 * Message Queue generation
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX  5

main(void)
{
  FILE    *fin;
  char     buffer[PIPE_BUF], u_char = 'A';
  int      i, n, mid[MAX];
  key_t    key;
  
  for ( i = 0; i < MAX; ++i, ++u_char) {
    key = ftok(".", u_char);
    
    if ( (mid[i] = msgget(key, IPC_CREAT | 0660)) == -1 ) {
      perror("Queue create");
      exit(1);
    }
  }
  
  fin = popen("ipcs", "r");	/* run the ipcs command */
  
  while ( (n = read(fileno(fin), buffer, PIPE_BUF)) > 0 )
    write(fileno(stdout), buffer, n); /* display ipcs output */
  
  pclose(fin);
  
  for ( i = 0; i < MAX; ++i )
    msgctl(mid[i], IPC_RMID, (struct msqid_ds *) 0); /* remove */
  exit(0);
}
