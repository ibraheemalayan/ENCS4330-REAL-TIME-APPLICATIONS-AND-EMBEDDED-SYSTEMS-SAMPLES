#include <stdio.h>       /* standard I/O routines                 */
#include <pthread.h>     /* pthread functions and data structures */

#define NUM_EMPLOYEES 2                   /* size of each array.    */

/* global mutex for our program. assignment initializes it */
pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

struct employee {
    int number;
    int id;
    char first_name[20];
    char last_name[30];
    char department[30];
    int room_number;
};

/* global variable - our employees array, with 2 employees */
struct employee employees[] = {
    { 1, 12345678, "coco", "mansoor", "Accounting", 101},
    { 2, 87654321, "layla", "lolo", "Programmers", 202}
};

/* global variable - employee of the day. */
struct employee employee_of_the_day;

/* function to copy one employee struct into another */
void
copy_employee(struct employee* from, struct employee* to)
{
    int rc;	/* contain mutex lock/unlock results */

    /* lock the mutex, to assure exclusive access to 'a' and 'b'. */
    rc = pthread_mutex_lock(&a_mutex);

    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;

    /* unlock mutex */
    rc = pthread_mutex_unlock(&a_mutex);
}

/* function to be executed by the variable setting threads thread */
void*
do_loop(void* data)
{
    int my_num = *((int*)data);   /* thread identifying number         */

    while (1) {
        /* set employee of the day to be the one with number 'my_num'. */
	copy_employee(&employees[my_num-1], &employee_of_the_day);
    }
}

/* like any C program, program's execution begins in main */
int
main(int argc, char* argv[])
{
    int        i;              /* loop counter                          */
    int        thr_id1;        /* thread ID for the first new thread    */
    int        thr_id2;        /* thread ID for the second new thread   */
    pthread_t  p_thread1;      /* first thread's structure              */
    pthread_t  p_thread2;      /* second thread's structure             */
    int        num1      = 1;  /* thread 1 employee number              */
    int        num2      = 2;  /* thread 2 employee number              */
    struct employee eotd;      /* local copy of 'employee of the day'.  */
    struct employee* worker;   /* pointer to currently checked employee */

    /* initialize employee of the day to first 1. */
    copy_employee(&employees[0], &employee_of_the_day);

    /* create a new thread that will execute 'do_loop()' with '1'       */
    thr_id1 = pthread_create(&p_thread1, NULL, do_loop, (void*)&num1);
    /* create a second thread that will execute 'do_loop()' with '2'    */
    thr_id2 = pthread_create(&p_thread2, NULL, do_loop, (void*)&num2);

    /* run a loop that verifies integrity of 'employee of the day' many */
    /* many many times.....                                             */
    for (i=0; i<60000; i++) {
      /* save contents of 'employee of the day' to local 'worker'.    */
      copy_employee(&employee_of_the_day, &eotd);
      worker = &employees[eotd.number-1];
        /* compare employees */
      if (eotd.id != worker->id) {
	printf("mismatching 'id' , %d != %d (loop '%d')\n",
	       eotd.id, worker->id, i);
	exit(0);
      }
      if (strcmp(eotd.first_name, worker->first_name) != 0) {
	printf("mismatching 'first_name' , %s != %s (loop '%d')\n",
	       eotd.first_name, worker->first_name, i);
	exit(0);
      }
      if (strcmp(eotd.last_name, worker->last_name) != 0) {
	printf("mismatching 'last_name' , %s != %s (loop '%d')\n",
	       eotd.last_name, worker->last_name, i);
	exit(0);
      }
      if (strcmp(eotd.department, worker->department) != 0) {
	printf("mismatching 'department' , %s != %s (loop '%d')\n",
	       eotd.department, worker->department, i);
	exit(0);
      }
      if (eotd.room_number != worker->room_number) {
	printf("mismatching 'room_number' , %d != %d (loop '%d')\n",
	       eotd.room_number, worker->room_number, i);
	exit(0);
      }
    }
    
    printf("Glory, employees contents were always consistent\n");
    
    return 0;
}
