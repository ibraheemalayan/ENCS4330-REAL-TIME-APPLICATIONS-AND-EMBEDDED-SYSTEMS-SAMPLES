/*
 * Displaying the contents of argv[] (the command line)
 */
#include <stdio.h>

int main(int argc, char *argv[])
{	
  for ( ; *argv; ++argv)
    printf("%s\n", *argv);
  
  printf("argc = %d\n", argc);
  
  return(0);
}
