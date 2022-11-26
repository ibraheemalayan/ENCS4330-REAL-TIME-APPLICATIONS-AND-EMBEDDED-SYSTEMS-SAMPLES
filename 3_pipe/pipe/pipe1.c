/*
 * Using a pipe to send data from parent to a child
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int  f_des[2];
  static char message[BUFSIZ];

  if  ( argc != 2 ) {
    fprintf(stderr, "Usage: %s message\n", *argv);
    exit(1);
  }

  if ( pipe(f_des) == -1 ) {
    perror("Pipe");
    exit(2);
  }
  
  switch ( fork() ) {
  case -1:
    perror("Fork");
    exit(3);

  case 0:			/* In the child */
    close(f_des[1]);
    if ( read(f_des[0], message, BUFSIZ) != -1 ) {
      printf("Message received by child: [%s]\n", message);
      fflush(stdout);
    }
    else {
      perror("Read");
      exit(4);
    }
    break;
    
  default:			/* In the parent */
    close(f_des[0]);
    //sleep(5);
    if ( write(f_des[1], argv[1], strlen(argv[1])) != -1 ) {
      printf("Message sent by parent: [%s]\n", argv[1]);
      fflush(stdout);
    }
    else {
      perror("Write");
      exit(5);
    }
  }
  exit(0);
}
