/* ************************* File Header **************************
 * 
 * File Name: net_serv.c
 * 
 * Revision History 
 * ________________ 
 * Date      	Author    	Description 
 * 
 * Mar-27-2005	Hanna Bullata, PhD
 * 
 *************************************************************** */

#include <stdio.h>
#include <sys/errno.h>
#include <libgen.h>
#include "net_header.h"
#include "net_serv.h"
#include "net_serv_funcs.h"
#include "error.h"
#include "read_write.h"

char  buf[MAXSIZE];
char  buf2[10][MAXSIZE];
char  errFileName[80];
char  ErrMsg[1024];
Args_Type Args;

extern FuncTable serverFuncs[];
extern int errno;

static int num_of_errors;
static int num_of_warnings;
static FILE *err_file;

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

  if ( Args.version ) {
    printf("\n");
    epulseHeader(argv[0]);
    printf("%s\n\n", PROG_VERSION);
  }
  
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

/*
 * getSocket(): Initializing socket communication (orig_sock)
 */

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
  
  /*
   * Checking if the service name is provided.
   */
  
  if ( strlen(Args.service) == 0 ) { /* checking idsutil service */
    
    serv = getservbyname(SERVICE_NAME, protocol);
    
    if ( serv != (struct servent *) NULL ) {
      port = ntohs(serv->s_port);
      printf("Connecting through service %s (port: %d)\n",
	     serv->s_name, serv->s_port);
    }
    
    else {
      port = PORT;
      printf("Connecting through alternative port (port: %d)\n", port);
    }
  }
  
  else {			/* checking user-defined service */
    
    serv = getservbyname(Args.service, protocol);
    
    if ( serv != (struct servent *) NULL) {
      port = ntohs(serv->s_port);
      printf("\nConnecting through user-defined service %s (port: %d)\n", 
	     serv->s_name, serv->s_port);
    }
    
    else {
      printf("\nError: could not connect to service %s\n", Args.service);
      exit(-1);
    }
  }
  
  /*
   * Checking the port number.
   */
  
  if ( Args.service_port != -1 ) {
    
    serv = getservbyport(Args.service_port, protocol);
    
    if ( serv != (struct servent *) NULL ) {
      
      if ( strlen(Args.service) != 0 ) {
	
	if ( port != serv->s_port ) {
	  printf("\nError: service name %s and port %d do not martch!\n",
		 Args.service, serv->s_port);
	  exit(-1);
	}
      }
      
      else {
	port = ntohs(serv->s_port);
	printf("\nConnecting through service %s (port: %d)\n",
	       serv->s_name, serv->s_port);
      }
    }
    
    else {
      printf("Error: could not connect to port %d\n", Args.service_port);
      exit(-1);
    }
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

  if ( (len = ReadLine(new_sock, buf, MAXSIZE)) > 0 ) { /*read client cmd */
    
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

/*
 * HandleOptions(): Handling the options
 */

static int HandleOptions(int argc, char *argv[])
{
  int i = 1;
  
  /*
   * Initializing the different fields of the Args structure.
   */
  
  Args.info = 0;
  Args.version = 0;

  memset(Args.service, 0, sizeof(char) * strlen(Args.service));
  
  Args.service_port = -1;

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
      if ( i < (argc - 1) ) 
	i++;
      else {
	printf("ERROR: Missing service name\n");
	return 0;
      }
      strncpy(Args.service, argv[i], SERV_LEN - 1);
      Args.service[SERV_LEN - 1] = '\0';
    }
    
    else if ( ! strcmp(argv[i], PORT_FLAG) ) {
      if ( i < (argc - 1) )
	i++;
      else {
	printf("ERROR: Missing port number\n");
	return 0;
      }
      Args.service_port = atoi(argv[i]);
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
  char *tmpName;
  
  printf("\n");
  
  tmpName = basename(name);
  
  epulseHeader(name);
  printf("%s %s\n", tmpName, PROG_VERSION);
  printf("\n");
  printf("Default usage:\n\n");
   printf("  %s \n\n", tmpName);
  printf("Usage:\n\n");
  printf("  %s [-i] [-v] [-s <service name>] [-p <port number>]\n\n", 
	 tmpName);
  printf("  Where:\n");
  printf("    -v      : Print version number of %s.\n", tmpName);
  printf("    -i      : Show this information.\n");
  printf("    %s      : Followed by a service name to establish TCP/IP \n",
	 SERVICE_FLAG);
  printf("              connection. If this option is ignored, by \n"
	 "              default the connection will be attempted\n"
	 "              on service \"%s\" (in /etc/services).\n", 
	 SERVICE_NAME);
  printf("    %s      : Followed by a port number to establish TCP/IP \n",
	 PORT_FLAG);
  printf("              connection. If this option is ignored and service \n"
	 "              \"%s\" not found, then by default the connection \n"
	 "              will be attempted on port %d.\n", 
	 SERVICE_NAME, PORT);
  printf("\n");
}

void epulseHeader(char *name)
{
  printf("E-Pulse Automation  - %s utility\n", basename(name));
}
