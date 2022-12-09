h12550
s 00003/00000/00428
d D 1.2 01/08/17 10:23:04 hanna 2 1
c Addition of the header file read_write.h
e
s 00428/00000/00000
d D 1.1 01/08/16 21:03:18 hanna 1 0
c date and time created 01/08/16 21:03:18 by hanna
e
u
U
f e 0
t
T
I 1
/* **************************************************************************
 * Copywrite (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
 *                No part of this program may be photocopied, reproduced,
 *                or translated to another programming language without the
 *                prior written consent of Innovative Data Solutions Inc.
 ************************************************************************** */


/* ****************************** File Header *******************************
 * 
 * File Name: net_serv.c
 * 
 * Description: 
 * 
 * Sccs Id:    %W% %G% %U%
 * 
 * Related Files/Documents: 
 * 
 * 
 * Revision History 
 * ________________ 
 * Date      	Author    	Description 
 * 
 * Jun-20-2001	Hanna Bullata & Bassem El-Sayrafi	
 * 
 ************************************************************************** */

I 2
#include <stdio.h>
#include <sys/errno.h>
E 2
#include "net_header.h"
#include "net_serv.h"
#include "net_serv_funcs.h"
I 2
#include "read_write.h"
E 2
#include "error.h"


char buf[MAXSIZE];
char buf2[10][MAXSIZE];
char  errFileName[80];
char      ErrMsg[1024];
Args_Type Args;

extern FuncTable serverFuncs[];
extern int errno;


int main(int argc, char *argv[]) 
{
  int orig_sock, res;
  
  /*
   * Handling the different options.
   */
  
  if ( HandleOptions(argc, argv) == 0 )
    exit (-1);
  
  /*
   * Creating the error log file.
   */
  
  sprintf(errFileName, "%s.log", argv[0]);
  res = init_error_log(errFileName);
  
  if ( res == ERR_FAILURE )
    exit (1);
  
  close_error_log();

  if ( Args.version )
    printf("  %s\n\n", PROG_VERSION);
  
  else if ( Args.info ) 
    PrintUsage(argv[0]);
  
  else {
    
    /*
     * Initializing socket communication.
     */
    
    orig_sock = getSocket();
    
    if ( orig_sock <= 0 )
      exit(5);
    
    /*
     * Creating the signal handlers.
     */

    sigset(SIGCHLD, signalHandler);
    sigset(SIGTERM, signalHandler);
    sigset(SIGTSTP, signalHandler); 
  
    printf("Server Running. \n");
    
    /*
     * Handling communication with clients.
     */
    
    res = clientCommunicate(orig_sock);
  }

  return res;
}

/*------------------------------------------------------------*/
/*                                                            */
/* getSocket(): Initializing socket communication (orig_sock) */
/*                                                            */
/*------------------------------------------------------------*/

int getSocket()
{
  int orig_sock = 0;
  int res = 0;
  char *Msg;
  char protocol[10];
  struct servent *serv;
  int port;

  static struct sockaddr_in  serv_adr;
  
  sprintf(protocol, "tcp");

  if ( (orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {

    sprintf(ErrMsg, "getSocket(): General Error");
    
    if ( errno > 0 ) {
      Msg = strerror(errno);
      sprintf(ErrMsg, "%s %s", ErrMsg, Msg);
    }
    
    res = reopen_error_log(errFileName);
    log_error(ErrMsg);
    close_error_log();
    exit(1);
  }
  
  port = PORT;
  
  if ( Args.service[0] != '\0' ) {
    
    serv = getservbyname(Args.service, protocol);
    
    if ( serv != (struct servent *) NULL)
      port = ntohs(serv->s_port);
  }
  
  memset(&serv_adr,0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(port);
  
  if ( bind(orig_sock, (struct sockaddr *) &serv_adr, 
	    sizeof(serv_adr)) < 0 ) {
    
    sprintf(ErrMsg, "getSocket(): Bind Error");
    
    if ( errno > 0 ) {
      Msg = strerror(errno);
      sprintf(ErrMsg, "%s %s", ErrMsg, Msg);
    } 
    
    res = reopen_error_log(errFileName);
    log_error(ErrMsg);
    close(orig_sock);
    close_error_log();
    exit(2);
  }
  
  if ( listen(orig_sock, 5) < 0 ) {
    sprintf(ErrMsg, "getSocket(): listen Error");

    if ( errno > 0 ) {
      Msg = strerror(errno);
      sprintf(ErrMsg, "%s %s", ErrMsg, Msg);
    } 

    res = reopen_error_log(errFileName);
    log_error(ErrMsg);
    close(orig_sock);
    
    close_error_log();
    exit(3);
  }
  
  return orig_sock;
}

/*------------------------------------------------------------*/
/*                                                            */
/* clientCommunicate(): Handling communication with clients   */
/*                                                            */
/*------------------------------------------------------------*/

int clientCommunicate(int orig_sock)
{
  static struct sockaddr_in clnt_adr;
  char *Msg;
  int 
    new_sock = 0,
    clnt_len = 0,
    res = 0;

  do 
    {
      clnt_len = sizeof(clnt_adr);
      
      if ( (new_sock = accept(orig_sock, 
			      (struct sockaddr *) &clnt_adr, 
			      &clnt_len)) < 0 ) {
	if ( errno == EINTR ) {
	  continue;
	}
	
	else {     
	  sprintf(ErrMsg, "clientCommunicate(): Accept Error");
	  
	  if ( errno > 0 ) {
	    Msg = strerror(errno);
	    sprintf(ErrMsg, "%s %s", ErrMsg, Msg);
	  } 
	  
	  res = reopen_error_log(errFileName);
	  log_error(ErrMsg);
	  close_error_log();
	  
	  close(orig_sock);
	  exit(4);
	}
      }
      
      if ( fork() == 0 ) {
	
	memset(buf, 0, MAXSIZE * sizeof(char));
	
	res = handleClientRequest(new_sock);	 
	
	close(new_sock);
	close(orig_sock);
	exit(0);
      }
      else {
	close(new_sock);
      }
      
    } while(1);
  
  close(new_sock);
  close(orig_sock);
}

/*------------------------------------------------------------*/
/*                                                            */
/* handleClientRequest(): Handling a request from a client    */
/*                                                            */
/*------------------------------------------------------------*/

int handleClientRequest(int new_sock) 
{
  int len = 0, tool = 0, res = 0;
  
  sigset(SIGTERM, signalHandler);

  if ( (len = ReadLine(new_sock, buf, MAXSIZE)) >0 ) { /*read client cmd */
    
    tool = atoi(buf);
    
    /*
     * Checking if the function is available. It should be!
     */
    
    if ( serverFuncs[tool-1].func_hdl != NULL ) {
	sprintf(buf, "SUCCESS\n");
	
	/*
	 * Sending SUCCESS to client.
	 */
	
	Writen(new_sock, buf, strlen(buf)*sizeof(char));
	
	/*
	 * calling the right function.
	 */
	
	res = (*(serverFuncs[tool-1].func_hdl))(new_sock);
      }
   
      else {
	
	sprintf(buf, "FAILURE\n");
	
	/*
	 * Sending FAILURE to client.
	 */
	
	Writen(new_sock, buf, strlen(buf)*sizeof(char));
	return res;
      }
    }
      
  return 1;
}

/*------------------------------------------------------------*/
/*                                                            */
/* signalHandler(): Signal handler                            */
/*                                                            */
/*------------------------------------------------------------*/

void signalHandler(int sig)
{
  pid_t pid, t;
  int stat=0;
  int res =0;
  
  t = -1;
  
  switch(sig) {
    
  case SIGCHLD:      
    
    while( (pid = waitpid(t, &stat, WNOHANG)) > 0 ) {
      sprintf(ErrMsg, "signalHandler(): child %ld terminated", (long) pid);
      
      res = reopen_error_log(errFileName);
      log_warning(ErrMsg);
      close_error_log();
    }
    break;
    
  case SIGTERM:
    exit(1);
	
  case SIGTSTP:
    sprintf(ErrMsg, "Killing Children");
    res = reopen_error_log(errFileName);
    log_warning(ErrMsg);
    close_error_log();
    
    kill(0,SIGTERM);
    break;
  }
  
  return;
}

/*------------------------------------------------------------*/
/*                                                            */
/* HandleOptions(): Handling the options.                     */
/*                                                            */
/*------------------------------------------------------------*/

static int HandleOptions(int argc, char *argv[])
{
  int i = 1;
  
  Args.info = 0;
  Args.version = 0;
  memset(Args.service, 0, sizeof(char)*strlen(Args.service) );
  Args.service[0] = '\0';

  if ( argc == 1 ) {
    PrintUsage(argv[0]);
    return 1;
  }
  
  while ( i < argc ) {
    
    if ( ! strcmp(argv[i], HELP_FLAG) ) {
      PrintUsage(argv[0]);
      return 0;
    }
    
    else if ( ! strcmp(argv[i], INFO_FLAG) ) {
      Args.info = 1;
    }
    
    else if ( ! strcmp(argv[i], SERVICE_FLAG) ) {
      if ( i < (argc - 1)) 
	i++;
      else {
	printf("ERROR: Missing service name\n");
	return 0;
      }
      strncpy(Args.service, argv[i], SERV_LEN - 1);
      Args.service[SERV_LEN - 1] = '\0';
    }
    
    else if ( ! strcmp(argv[i], VERSION_FLAG) ) {
      Args.version = 1;
    }
    
    else {
      PrintUsage(argv[0]);
      return 0;
    }
    i++;
  }
  
  return 1;
}

/*------------------------------------------------------------*/
/*                                                            */
/* PrintUsage(): Printing the usage.                          */
/*                                                            */
/*------------------------------------------------------------*/

static void PrintUsage(char *name)
{
  printf("\n");
  printf("IDS Software Systems - idsserver utility\n");
  printf("%s %s\n", name, PROG_VERSION);
  printf("\n");
  printf("Usage:\n");
  printf("  %s [-i] [-v] \n", name);
  printf("or \n");
  printf("  %s [-s <service name>] \n\n", name);
  printf("  Where:\n");
  printf("    -v      : Print version number of %s.\n", name);
  printf("    -i      : Show this information.\n");
  printf("    %s      : Followed by a service name to establish TCP/IP \n",
	 SERVICE_FLAG);
  printf("              connection. If this argument is not specified, \n"
	 "              then by default the connection will be attempted\n"
	 "              on port %d.\n", PORT);
  printf("\n");
}
E 1
