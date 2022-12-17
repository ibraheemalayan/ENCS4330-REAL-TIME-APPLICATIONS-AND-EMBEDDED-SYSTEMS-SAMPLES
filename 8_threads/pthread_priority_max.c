/*
 * Getting the priority range defined under Linux
 */
#include <stdio.h>  
#include <pthread.h>
#include <sched.h>

int max_prio, min_prio;

int main()
{
  max_prio = sched_get_priority_max(SCHED_FIFO);
  min_prio = sched_get_priority_min(SCHED_FIFO);

  printf("min_prio: %d\t max_prio = %d\n", min_prio, max_prio);
}
