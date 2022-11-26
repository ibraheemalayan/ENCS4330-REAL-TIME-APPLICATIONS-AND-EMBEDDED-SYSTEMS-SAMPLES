#ifndef __LOCAL_H__
#define __LOCAL_H__

/*
 * Common header file for Message Queue Example
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define SEED   'g'		/* seed for ftok */
#define SERVER 1L		/* message for the server */

typedef struct {
  long      msg_to;		/* Placed in the queue for */
  long      msg_fm;		/* Placed in the queue by  */
  char      buffer[BUFSIZ];
} MESSAGE;

#endif
