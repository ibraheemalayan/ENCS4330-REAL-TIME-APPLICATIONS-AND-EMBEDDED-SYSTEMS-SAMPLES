/*
  Running the cat utility via an exec system call
  */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[ ])
{
  if ( argc != 2 ) {
    printf("Usage: %s fileName\n", argv[0]);
    exit(-1);
  }
  
  execlp("/bin/catt", "catt", argv[1], (char *) NULL);
  perror("exec failure ");
  exit(-2);
}
