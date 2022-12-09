/*
 * Client - UNIX domain, connection-oriented
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define NAME "my_sock"

main(void)
{
  int     orig_sock,	/* Original socket descriptor in server */
          i;		/* Loop counter */
  pid_t myPid;
  
  static struct sockaddr_un 
                   serv_adr;/* UNIX addresses of the server process */

  static char buf[10];		/* Buffer for messages */

  myPid = getpid();
  
  printf("Client pid = %ld\n\n", myPid);
  
  if ( (orig_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
    perror("generate error");
    exit(1);
  }
  
  serv_adr.sun_family = AF_UNIX;
  strcpy(serv_adr.sun_path, NAME);

  if ( connect(orig_sock, (struct sockaddr *) &serv_adr,
	    sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0 ) {
    perror("connect error");
    exit(1);
  }
  
  for ( i = 1; i <= 10; i++ ) {	/* Send msgs */
    sprintf(buf, "c: %d\n", i);
    write(orig_sock, buf, sizeof(buf));
  }
  
  sprintf(buf, "%d", myPid);
  write(orig_sock, buf, sizeof(buf));
  
  close(orig_sock);
  exit(0);
}
