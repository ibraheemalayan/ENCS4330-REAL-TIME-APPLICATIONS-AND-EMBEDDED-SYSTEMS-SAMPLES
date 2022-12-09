/*
 * Server - UNIX domain, connection-oriented
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>		/* as we are using UNIX protocol */

#define NAME "my_sock"

main(void)
{
  int orig_sock,      /* Original socket descriptor in server */
      new_sock,	      /* New socket descriptor in server */
      clnt_len,	      /* Length of client address */
      i;	      /* Loop counter */

  static struct sockaddr_un 
                   clnt_adr,	/* UNIX addresses of client-server */
                   serv_adr;

  static char buf[10];		/* Buffer for messages */

  void clean_up(int, char *);	/* Close socket and remove routine */

  /*
   * Program starts here
   */
  
  if ( (orig_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
    perror("generate error");
    exit(1);
  }
  
  serv_adr.sun_family = AF_UNIX;
  strcpy(serv_adr.sun_path, NAME);
  unlink(NAME);

  if ( bind(orig_sock, (struct sockaddr *) &serv_adr,
	    sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0 ) {
    perror("bind error");
    clean_up(orig_sock, NAME);
    exit(2);
  }
  
  listen(orig_sock, 1);		/* Listen */

  clnt_len = sizeof(clnt_adr);

  if ( (new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
			  &clnt_len)) < 0 ) { /* Accept */
    perror("accept error");
    clean_up(orig_sock, NAME);
    exit(3);
  }

  for ( i = 1; i <= 10; i++ ) {	/* Process */
    sleep(1);
    read(new_sock, buf, sizeof(buf));
    printf("s-> %s", buf);
  }
  close(new_sock);
  clean_up(orig_sock, NAME);
  exit(0);
}

void clean_up(int sd, char *the_file)
{
  close(sd);			/* close it */
  unlink(the_file);		/* rm it */
}
