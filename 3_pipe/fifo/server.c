/*
 * The server process
 */

#include "local.h"

void main(void)
{
  int            n, done, dummyfifo, publicfifo, privatefifo;
  struct message msg;
  FILE          *fin;
  static char    buffer[PIPE_BUF];

  /* Generate the public FIFO */
  if ( (mknod(PUBLIC, S_IFIFO | 0666, 0)) == -1) {
    perror("Error");
    exit(-1);
  }
  
  /* Open the public FIFO for reading and writing */
  if ( (publicfifo = open(PUBLIC, O_RDONLY)) == -1 ||
       (dummyfifo  = open(PUBLIC, O_WRONLY | O_NDELAY)) == -1 ) {
    perror(PUBLIC);
    exit(1);
  }
  
  /* message can be read from the PUBLIC pipe */
  while ( read(publicfifo, (char *) &msg, sizeof(msg)) > 0 ) {
    n = done = 0;		/* clear counters / flags */
    do {
      if ( (privatefifo = open(msg.fifo_name, O_WRONLY | O_NDELAY)) == -1 )
	sleep(3);
      else {
	fin = popen(msg.cmd_line, "r");	/* execute the cmd */
	write(privatefifo, "\n", 1); /* keep output pretty */

	while ( (n = read(fileno(fin), buffer, PIPE_BUF)) > 0 ) {
	  write(privatefifo, buffer, n); /* to private fifo */
	  memset(buffer, 0x0, PIPE_BUF); /* clear it out */
	}
	pclose(fin);
	close(privatefifo);
	done = 1;
      }
    } while ( ++n < 5 && ! done);
    
    if ( ! done )
      write(fileno(stderr), "\nNOTE: SERVER ** NEVER ** accessed private fifo\n", 48);
  }
}

