/* **************************************************************************
 * Copyright (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
 *                No part of this program may be photocopied, reproduced,
 *                or translated to another programming language without the
 *                prior written consent of Innovative Data Solutions Inc.
 ************************************************************************** */


/* ****************************** File Header *******************************
 * 
 * File Name: server.c
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
 * Feb-28-2001	Hanna Bullata	
 * 
 ************************************************************************** */

#include "super_local.h"

/* communication shell - C language */
static int fildes[2];

main(void)
{
  int orig_sock, new_sock, clnt_len, count=0, option, archive;
  int licenseStatus;
  char cmd[BUFSIZ];

  struct sockaddr_in clnt_adr, serv_adr;
  int len, i;
  
  if ( pipe(fildes) != 0 ) {
    perror("pipe");
    exit(-1);
  }
  
  if ( (orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("generate error");
    exit(1);
  }
  
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family      = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port        = htons(PORT);
  
  if ( bind(orig_sock, (struct sockaddr *) &serv_adr,
	    sizeof(serv_adr)) < 0 ) {
    perror("bind error");
    close(orig_sock);
    exit(2);
  }
  
  if ( listen(orig_sock, 5) < 0 ) {
    perror("listen error");
    close(orig_sock);
    exit(3);
  }
  
  do {
    clnt_len = sizeof(clnt_adr);
    
    if ( (new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
			    &clnt_len)) < 0 ) {
      perror("accept error");
      close(orig_sock);
      exit(4);
    }
    
    if ( fork() == 0 ) {	/* in the child */
      while ( (len = read(new_sock, buf, BUFSIZ)) > 0 ) {
	
	option = atoi(buf);
	printf("option = %d\n", option);
	
	switch ( option ) {
	  
	case UPTIME_REQUEST:
	  
	  sprintf(cmd, "uptime 1>&%d", fildes[1]);
	  
	  if ( system(cmd) ) {
	    sprintf(buf, "Getting uptime failed.");
	    write(new_sock, buf, strlen(buf)*sizeof(char));
	    break;
	  }
	  
	  while ( ! read(fildes[0], buf, BUFSIZ) );
	  
	  /* send result to client */
	  write(new_sock, buf, strlen(buf)*sizeof(char));
	  
	  break;
	  
	case ARCHIVE_STATUS_REQUEST:
	  
	  /* testing if archive is already active */
	  sprintf(cmd, "\\ps -ef | \\grep archive | \\awk \'{print $8}\' | \\awk \'/archive$/\' | \\wc -l 1>&%d", fildes[1]);
	  
	  if ( system(cmd) ) {
	    sprintf(buf, "Getting archive failed.");
	    write(new_sock, buf, strlen(buf)*sizeof(char));
	    break;
	  }
	  
	  while ( ! read(fildes[0], buf, BUFSIZ) );
	  
	  if ( (archive = atoi(buf)) == 0 ) /* archive not active */
	    sprintf(buf, "Archive is NOT active.");
	  
	  else if ( archive == 1 )
	    sprintf(buf, "Archive is ACTIVE.");
	  
	  else if ( archive > 1 )
	    sprintf(buf, "Archive has %d processes !!!", archive);
	  
	  write(new_sock, buf, strlen(buf)*sizeof(char));
	  break;
	  
	case LICENSES_STATUS_REQUEST:
	  
	  memset(buf, 0, BUFSIZ);
	  
	  /* testing if licenses are already active */
	  sprintf(cmd, "\\ps -ef | \\grep lmgrd | \\awk \'/[^ ]lmgrd$/\' | \\wc -l 1>&%d", fildes[1]);
	  
	  if ( system(cmd) ) {
	    sprintf(buf, "Getting license status failed.");
	    write(new_sock, buf, strlen(buf)*sizeof(char));
	    break;
	  }
	  
	  while ( ! read(fildes[0], buf, BUFSIZ) );
	  
	  if ( (licenseStatus = atoi(buf)) == 1 ) { /* licenses are on */
	    
	    sprintf(cmd, "\\ps -ef | \\grep lmgrd | \\awk \'/[^ ]lmgrd$/\' | \\awk \'{print $7}\' 1>&%d", fildes[1]);
	    
	    if ( system(cmd) ) {
	      sprintf(buf, "Getting license status failed.");
	      write(new_sock, buf, strlen(buf)*sizeof(char));
	      break;
	    }
	    
	    while ( ! read(fildes[0], buf, BUFSIZ) );
	    
	    if ( strncmp(buf, "?", 1) == 0 ) {
	      
	      sprintf(cmd, "\\ps -ef | \\grep lmgrd | \\awk \'/[^ ]lmgrd$/\' | \\awk \'{print $5 \" \" $6}\' 1>&%d", fildes[1]);
	      
	      if ( system(cmd) ) {
		sprintf(buf, "Getting license status failed.");
		write(new_sock, buf, strlen(buf)*sizeof(char));
		break;
	      }
	      
	      while ( ! read(fildes[0], buf, BUFSIZ) );
	    }
	    
	    else {

	      sprintf(cmd, "\\ps -ef | \\grep lmgrd | \\awk \'/[^ ]lmgrd$/\' | \\awk \'{print $4}\' 1>&%d", fildes[1]);
	      
	      if ( system(cmd) ) {
		sprintf(buf, "Getting license status failed.");
		write(new_sock, buf, strlen(buf)*sizeof(char));
		break;
	      }
	      
	      while ( ! read(fildes[0], buf, BUFSIZ) );
	      
	    }

	    sprintf(buf, "%s -> licenses are ON.", buf);
	  }
	  
	  else if ( licenseStatus > 1 )
	    sprintf(buf, "Strange: Number of lmgrd is %d.", licenseStatus);
	  
	  else
	    sprintf(buf, "Licenses are OFF.");
	  
	  write(new_sock, buf, strlen(buf)*sizeof(char));
	  break;
	  
	case ARCHIVE_REQUEST:
	  
	  /* testing if archive is already active */
	  sprintf(cmd, "\\ps -ef | \\grep server | \\awk \'{print $8}\' | \\awk \'/archive$/\' | \\wc -l 1>&%d", fildes[1]);
	  
	  if ( system(cmd) ) {
	    sprintf(buf, "Getting archive failed.");
	    write(new_sock, buf, strlen(buf)*sizeof(char));
	    break;
	  }
	  
	  while ( ! read(fildes[0], buf, BUFSIZ) );
	  
	  if ( (archive = atoi(buf)) == 0 ) { /* archive not active */
	    
	    sprintf(cmd, "/export/home/hanna/script/admin/archive");
	    
	    if ( system(cmd) ) {
	      sprintf(buf, "Starting archive failed!");
	      write(new_sock, buf, strlen(buf)*sizeof(char));
	      break;
	    }
	  }

	  else if ( archive == 1 ) {
	    sprintf(buf, "archive is active!");
	    write(new_sock, buf, strlen(buf)*sizeof(char));
	    break;
	  }
	  
	  else {
	    sprintf(buf, "archive problem: %d processes active!", archive);
	    write(new_sock, buf, strlen(buf)*sizeof(char));
	    break;
	  }
	  
	default:
	  break;
	}
      }
      
      close(new_sock);
      exit(0);
    }
    else			/* in the parent */
      close(new_sock);
  } while (1);
}
