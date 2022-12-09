/*
 * Creating a socket pair
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SZ 10

main(void)
{
  int         sock[2];		/* The socket pair */
  int         cpid, i;
  static char buf[BUF_SZ];	/* Temporary buffer for message */

  if ( socketpair(PF_UNIX, SOCK_STREAM, 0, sock) < 0 ) {
    perror("Generation error");
    exit(1);
  }
  
  switch ( cpid = (int) fork() ) {
  case -1:
    perror("Bad fork");
    exit(2);
    
  case 0:			/* The child process */
    close(sock[1]);
    for ( i = 0; i < 10; i += 2 ) {
      sleep(1);
      sprintf(buf, "c: %d\n", i);
      write(sock[0], buf, sizeof(buf));
      read(sock[0], buf, BUF_SZ);
      printf("c -> %s", buf);	/* Message from parent */
    }
    close(sock[0]);
    break;

  default:			/* The parent process */
    close(sock[0]);
    for ( i = 1; i < 10; i += 2 ) {
      sleep(1);
      read(sock[1], buf, BUF_SZ);
      printf("p -> %s", buf);	/* Message from child */
      sprintf(buf, "p: %d\n", i);
      write(sock[1], buf, sizeof(buf));
    }
    close(sock[1]);
  }

  return(0);
}
