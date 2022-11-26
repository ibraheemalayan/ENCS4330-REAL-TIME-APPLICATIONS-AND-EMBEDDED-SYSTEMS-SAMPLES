/*
 * Displaying the contents of argv[] (the command line)
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  float angle, dodo;
  
  if ( argc < 2 ) {
    printf("Please give an angle\n");
    exit(-1);
  }
  
  if ( argc > 2 ) {
    printf("Please give only an angle\n");
    exit(-1);
  }

  angle = atof(argv[1]);
  
  printf("cos(%f) = %f\n", angle, cos(angle));

  return 0;
}
