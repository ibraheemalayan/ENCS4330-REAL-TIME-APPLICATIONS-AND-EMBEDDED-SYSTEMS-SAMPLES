/* **************************************************************************
 * Copyright (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
 *                No part of this program may be photocopied, reproduced,
 *                or translated to another programming language without the
 *                prior written consent of Innovative Data Solutions Inc.
 ************************************************************************** */


/* ****************************** File Header *******************************
 * 
 * File Name: read_write.h
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
 * Aug-17-2001	Hanna Bullata	
 * 
 ************************************************************************** */


#ifndef _READ_WRITE_H_
#define _READ_WRITE_H_

int my_read(int fd, char *ptr);
int readLine(int fd, void *ptr, size_t maxlen);
size_t writen(int fd, const void *vptr, size_t n);
void Writen(int fd, const void *vptr, size_t n);
int ReadLine(int fd, void *vptr, size_t maxlen);

#endif
