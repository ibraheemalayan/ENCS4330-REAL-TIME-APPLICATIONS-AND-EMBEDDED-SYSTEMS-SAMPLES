/* **************************************************************************
 * Copywrite (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
 *                No part of this program may be photocopied, reproduced,
 *                or translated to another programming language without the
 *                prior written consent of Innovative Data Solutions Inc.
 ************************************************************************** */


/* ****************************** File Header *******************************
 * 
 * File Name: net_header.h
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
 * Mar-26-2001	Hanna Bullata & Bassem El-Sayrafi	
 * 
 ************************************************************************** */

/* 
 * This file contains the main header files that will be included in both
 * the server and the client.
 */

#ifndef _NET_HEADER_H_
#define _NET_HEADER_H_

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<dirent.h>
#include<sys/wait.h>
#include<sys/errno.h>
#include<signal.h>


#define MAXSIZE 1024
#define ROWS 10
#define PORT 6999
#define FILESIZE 96
#define SERVICE_NAME "idsutil"

/* 
 * Structure FuncTable will contain the main functions to be called
 * by the server and the client according to options
 */

typedef struct _FuncTable {
  long (*func_hdl)(int);
} FuncTable;

#endif
