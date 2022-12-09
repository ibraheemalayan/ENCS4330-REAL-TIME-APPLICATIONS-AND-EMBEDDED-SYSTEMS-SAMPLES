/* ************************* File Header **************************
 * 
 * File Name: net_serv.h
 * 
 * Revision History 
 * ________________ 
 * Date      	Author    	Description 
 * 
 * Mar-27-2005	Hanna Bullata, PhD	
 * 
 **************************************************************** */

#ifndef _NET_SERV_H_
#define _NET_SERV_H_

int getSocket();
int clientCommunicate(int orig_sock);
int handleClientRequest(int new_sock);
void signalHandler(int sig);
static void PrintUsage(char *name);
static int HandleOptions(int argc, char *argv[]);
void epulseHeader(char *name);

#define PROG_VERSION "Version 1.0 - March 27, 2005"
#define SERV_LEN     64
#define HELP_FLAG    "-u"
#define INFO_FLAG    "-i"
#define VERSION_FLAG "-v"
#define SERVICE_FLAG "-s"
#define PORT_FLAG    "-p"

typedef struct _args {
  int  info;
  int  version;
  char service[64];
  int  service_port;
} Args_Type;

#endif

