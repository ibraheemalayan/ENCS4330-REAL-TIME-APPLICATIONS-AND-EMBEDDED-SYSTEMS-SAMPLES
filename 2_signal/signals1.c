/*
 * Catching signals with sigset
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  int  i;
  void signal_catcher(int);
  void signal_catcher1(int);
  void signal_catcher10(int);

  if ( sigset(2, signal_catcher) == SIG_ERR ) {
    perror("Sigset can not set SIGINT");
    exit(SIGINT);
    }
  if ( sigset(SIGQUIT, signal_catcher) == SIG_ERR ) {
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }
  if ( sigset(7, signal_catcher1) == SIG_ERR ) {
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }

  if ( sigset(SIGUSR1, signal_catcher10) == SIG_ERR ) {
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }

  /*if ( sigset(SIGKILL, signal_catcher10) == SIG_ERR ) {
    perror("Sigset can not set SIGKILL");
    exit(SIGKILL);
    }*/

  for ( i = 0; ; ++i ) {
    printf("%i\n", i);

    sleep(1);
  }
}

void signal_catcher(int the_sig)
{
  printf("\nSignal %d received.\n", the_sig);
  
  if ( the_sig == SIGQUIT )
    exit(1);
}

void signal_catcher1(int the_sig)
{
}

void signal_catcher10(int the_sig)
{
  printf("Youpiiiiiiiii!\n");
  return;
}

