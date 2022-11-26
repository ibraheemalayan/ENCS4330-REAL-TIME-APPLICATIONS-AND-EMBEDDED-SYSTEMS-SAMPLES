
/*
 * local header file for pipe client-server
 */

#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define PUBLIC "/tmp/PUBLIC"
#define B_SIZ (PIPE_BUF / 2)

struct message {
  char    fifo_name[B_SIZ];
  char    cmd_line[B_SIZ];
};

#endif

