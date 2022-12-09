/* ************************* File Header **************************
 * 
 * File Name: net_serv_funcs.h
 * 
 * Revision History 
 * ________________ 
 * Date      	Author    	Description 
 * 
 * Mar-27-2005	Hanna Bullata, PhD	
 * 
 ***************************************************************** */

#ifndef _NET_SERV_FUNCS_H
#define _NET_SERV_FUNCS_H

#include"net_header.h"

int my_read(int fd, char *ptr);
int readLine(int fd, void *ptr, size_t maxlen);
size_t writen(int fd, const void *vptr, size_t n);
void Writen(int fd, const void *vptr, size_t n);
int ReadLine(int fd, void *vptr, size_t maxlen);


int handleLS(int new_sock, char startDir[], char fltr[]);
int handleCD(int new_sock, char startDir[]);
int handlePWD(int new_sock, char startDir[]);
int getFiles(char *startDir, char buf[][MAXSIZE], 
		    int *rows, char *fltr);
int getFltr(char buf[], char *fltr);

int handleSeekInformix(int new_sock);
int getFileData();
int handleKillInformix(int new_sock);
int getPWD(char *startDir, char buf[]);
int handleKillIdsserver(int new_sock);

long hdl_seek_communication(int new_sock);
long hdl_seek_informix(int new_sock);
long hdl_kill_informix(int new_sock);
long hdl_kill_idsserver(int new_sock);

#endif
