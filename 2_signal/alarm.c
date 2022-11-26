/*
 * Catching signals with sigset
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int val = 0;

int main(void)
{
  int  i;
  void signal_catcher(int);
  
  alarm(1);
  
  if ( sigset(SIGALRM, signal_catcher) == SIG_ERR ) {
    perror("Sigset can not set SIGALRM");
    exit(SIGINT);
  }
  
  while ( 1 );
}

void signal_catcher(int the_sig)
{
  val++;
  
  printf("val = %d\n", val);

  if (val < 10)
    alarm(1);
}
