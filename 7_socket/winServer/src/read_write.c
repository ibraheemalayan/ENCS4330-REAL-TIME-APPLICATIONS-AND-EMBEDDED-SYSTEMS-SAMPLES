/* **************************************************************************
 * Copyright (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
 *                No part of this program may be photocopied, reproduced,
 *                or translated to another programming language without the
 *                prior written consent of Innovative Data Solutions Inc.
 ************************************************************************** */


/* ****************************** File Header *******************************
 * 
 * File Name: read_write.c
 * 
 * Description: 
 * 
 * Sccs Id:    @(#)read_write.c	1.1 08/17/01 10:25:50
 * 
 * Related Files/Documents: 
 * 
 * 
 * Revision History 
 * ________________ 
 * Date      	Author    	Description 
 * 
 * Aug-17-2001	Hanna Bullata & Bassem El-Sayrafi	
 * 
 ************************************************************************** */

#include <stdio.h>
#include <sys/errno.h>
#include "net_header.h"

extern int errno;

int my_read(int fd, char *ptr) 
{
  static int read_cnt = 0;
  static char *read_ptr;
  static char read_buf[MAXSIZE];
  
  if ( read_cnt <= 0 ) {

  again:
    if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0 ) {
      
      if ( errno == EINTR )
	goto again;
      
      return -1;
    }
    
    else if ( read_cnt == 0 )
      return 0;
    
    read_ptr = read_buf;
  }
  
  read_cnt--;
  *ptr = *read_ptr++;
  return 1;
}

int readLine(int fd, void *vptr, size_t maxlen) 
{
  int n, rc;
  char c, *ptr;
  
  ptr = vptr;
  
  for ( n = 1; n < maxlen; n++ ) {
    if ( (rc = my_read(fd, &c)) == 1 ) {
      *ptr++ = c;

      if ( c == '\n' )
	break;
    }
    
    else if ( rc == 0 ) {
      if ( n == 1 )
	return 0;
      else
	break;
    }
    else
      return -1;		/* errno set by my_read*/
  }

  *ptr = 0;
  return n;
}

size_t writen(int fd, const void *vptr, size_t n) 
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;
  
  ptr = vptr;
  nleft = n;
  
  while ( nleft > 0 ) {
    
    if ( (nwritten = write(fd, ptr, nleft)) <= 0 ) {
      if ( errno == EINTR )
	nwritten = 0;
      else
	return -1;
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return n;
}

void Writen(int fd, const void *vptr, size_t n) 
{
  if ( writen(fd, vptr, n) != n ) {
    printf("Error while writting to socket\n");
    exit(5);
  }
}

int ReadLine(int fd, void *vptr, size_t maxlen) 
{
  int n;
  
  if ( (n = readLine(fd, vptr, maxlen)) < 0 ) {
    printf("ReadLine Error\n");
    exit(6);
  }
  
  return n;
}
