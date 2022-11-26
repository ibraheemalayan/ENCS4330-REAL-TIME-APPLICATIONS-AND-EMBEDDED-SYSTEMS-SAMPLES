/*
 * The client process
 */

#include "local.h"

void main(void)
{
  int            n, privatefifo, publicfifo;
  static char    buffer[PIPE_BUF];
  struct message msg;

  /*Make the name for the private fifo */
  sprintf(msg.fifo_name, "/tmp/fifo%d", getpid());
  
  /* Generate the private FIFO */
  if ( mknod(msg.fifo_name, S_IFIFO | 0666, 0) < 0 ) {
    perror(msg.fifo_name);
    exit(1);
  }

  /* Open the public FIFO for writing */
  if ( (publicfifo = open(PUBLIC, O_WRONLY)) == -1 ) {
    perror(PUBLIC);
    exit(2);
  }
  
  while ( 1 ) {			/* FOREVER */
    write(fileno(stdout), "\ncmd>", 6);	/* prompt */
    memset(msg.cmd_line, 0x0, B_SIZ); /* clear first */
    n = read(fileno(stdin), msg.cmd_line, B_SIZ); /* Get cmd */

    if ( ! strncmp("quit", msg.cmd_line, n - 1) ) /* EXIT ? */
      break;
    
    write(publicfifo, (char *) &msg, sizeof(msg));
    
    /* Open private fifo to read returned command output */
    if ( (privatefifo = open(msg.fifo_name, O_RDONLY)) == -1 ) {
      perror("msg.fifo_name");
      exit(3);
    }
    
    /* Read private FIFO and display on standard error */
    while ( (n = read(privatefifo, buffer, PIPE_BUF)) > 0 )
      write(fileno(stderr), buffer, n);
    
    close(privatefifo);
  }
  
  close(publicfifo);
  unlink(msg.fifo_name);	/* Remove */
}
