/*
 * Pausing with sigpause
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void main(void)
{
  void   sigset_catcher(int);
  void   sigset_catcher2(int);
  void   sigset_catcherq(int n);
    
  sigset(SIGUSR2, sigset_catcher);
  sigset(SIGUSR1, sigset_catcher2);
  sigset(SIGQUIT, sigset_catcherq);

  while ( 1 ) {
    printf("Waiting for signal %d\n", SIGUSR1);
    sigpause(SIGUSR2);		/* Wait for USR1 signal */
  }
}

void sigset_catcher(int n)
{
  sighold(SIGINT);		/* Hold interrupt */
  sighold(SIGTERM);		/* Hold terminate */
  sighold(SIGQUIT);		/* Hold Quit */
  printf("Beginning important stuff\n");
  sleep(25);
  printf("Ending important stuff\n");
  sigrelse(SIGINT);		/* Release interrupt */
  sigrelse(SIGTERM);		/* Release terminate */
  sigrelse(SIGQUIT);		/* Release quit */
}

void sigset_catcher2(int n)
{

}

void sigset_catcherq(int n)
{
  printf("Received signal quit\n");
}
