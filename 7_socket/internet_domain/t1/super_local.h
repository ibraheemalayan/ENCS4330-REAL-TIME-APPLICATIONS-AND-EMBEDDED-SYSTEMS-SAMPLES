#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 6996

#define BIG_NUMBER 100000.0
#define UPTIME_REQUEST          1
#define ARCHIVE_REQUEST         2
#define ARCHIVE_STATUS_REQUEST  3
#define LICENSES_STATUS_REQUEST 4

static char buf[BUFSIZ];
