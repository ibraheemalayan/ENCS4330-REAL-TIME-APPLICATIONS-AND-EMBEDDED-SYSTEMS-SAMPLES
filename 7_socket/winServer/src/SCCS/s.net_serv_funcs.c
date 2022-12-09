h22022
s 00002/00104/00711
d D 1.2 01/08/17 10:24:08 hanna 2 1
c Removal of read/write functions to read_write.c file and addition of the header file read_write.h
e
s 00815/00000/00000
d D 1.1 01/08/16 21:03:21 hanna 1 0
c date and time created 01/08/16 21:03:21 by hanna
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
 * File Name: net_serv_funcs.c
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

D 2


#include "net_serv_funcs.h"
E 2
#include <stdio.h>
#include <sys/errno.h>
I 2
#include "net_serv_funcs.h"
#include "read_write.h"
E 2
#include "error.h"
#include "dbcommon.h"

extern int errno;
extern char errFileName[80];
extern char ErrMsg[1024];

extern char buf[MAXSIZE];
extern char buf2[10][MAXSIZE];


FuncTable serverFuncs[] = {
  {hdl_seek_communication},
  {hdl_seek_informix},
  {hdl_kill_informix},
  {hdl_kill_idsserver},
  {NULL}
};

D 2

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

E 2
long hdl_seek_communication(int new_sock) 
{
  int len = 0, res = 0;
  char fltr[32], startDir[512];
  
  fltr[0] = '\0';
  
  if  ( getenv("DPMAINPATH=") != NULL )
    sprintf(startDir,"%s/", getenv("DPMAINPATH="));
  
  else if ( getenv("HOME=") != NULL )
    sprintf(startDir,"%s/", getenv("HOME="));

  else
    sprintf(startDir,"/");
/*
  else {
    log_error("unable to find HOME or DPMAINPATH environment variables.");
    return -1;
  }        
  */
  
  while( (len = ReadLine(new_sock, buf, MAXSIZE)) > 0 )	{ /*read client cmd */
    
    if ( strncasecmp("ls", buf, 2) == 0 ) {
      res = handleLS(new_sock, startDir, fltr);	
      if ( res == -1)
	break;	
    }

    if ( strncasecmp("cd", buf, 2) == 0 ) {    
      res = handleCD(new_sock, startDir);
      if ( res == -1 )
	break;
    }
      
    if ( strncasecmp("pwd", buf, 3) == 0 ) {
      res = handlePWD(new_sock, startDir);
      if ( res == -1 )
	break;
    }
    
    if( buf[0] == '.' )
      break;
    
    memset(buf, 0, MAXSIZE * sizeof(char));
  }
  
  return 1;
}

int handleLS(int new_sock, char startDir[], char fltr[]) 
{
  int rows = 0, res = 0, len = 0, i = 0;
  
  fltr[0] = '\0';
  res = getFltr(buf, fltr);

  memset(buf, 0, MAXSIZE * sizeof(char)); /*init*/
  
  for ( i = 0; i < ROWS; i++ )
    memset(buf2[i], 0, MAXSIZE * sizeof(char));
  
  res = getFiles(startDir, buf2, &rows, fltr);	  

  if ( res > 0 )
    sprintf(buf, "SUCCESS\n");

  else
    sprintf(buf, "FAILURE: %s\n", ErrMsg);
  
  errno = 0;
  
  Writen(new_sock, buf, strlen(buf)*sizeof(char));  
  
  memset(buf, 0, MAXSIZE * sizeof(char)); /*init*/

  /*receive ?? or .*/
  len = ReadLine(new_sock, buf, MAXSIZE);      
  
  if ( buf[0] == '?' )  
    res = 1;

  else {
    buf[0] = ' ';
    return 1;
  }
  
  if ( res > 0 ) {
    
    for ( i = 0; i <= rows; i++ ) {	      
      
      /*send to client */
      Writen(new_sock, buf2[i], strlen(buf2[i])*sizeof(char));  
      
      /*receive confirm from client*/
      len = ReadLine(new_sock, buf, MAXSIZE);   	      

      if ( buf[0] != '?' ) {
	buf[0] = '!';
	break;
      }
    }
    
    if ( buf[0] == '!' )
      return -1;
  }
  
  /*send ! client */
  sprintf(buf, "!\n");
  Writen(new_sock, buf, strlen(buf)*sizeof(char));  
  buf[0] = ' ';
  
  return 1;
}

int handleCD(int new_sock, char startDir[])
{
  int res = 0;
  
  res = getPWD(startDir, buf);
  
  memset(buf, 0, MAXSIZE * sizeof(char));
  
  if ( res > 0 )
    sprintf(buf, "SUCCESS\n");
  
  else
    sprintf(buf, "FAILURE: %s\n", ErrMsg);
	
  /*write success*/
  Writen(new_sock, buf, strlen(buf)*sizeof(char));       
  
  /*get confirm*/
  ReadLine(new_sock, buf, MAXSIZE);

  /* if didn't receive confirm*/
  if ( buf[0] != '?' )
    return -1;
  
  /*write '.'*/
  sprintf(buf, ".\n");
  Writen(new_sock, ".\n", strlen(buf)*sizeof(char));                  
  buf[0] = ' ';
  
  return 1;
}

int handlePWD(int new_sock, char startDir[])
{
  memset(buf, 0, MAXSIZE * sizeof(char));
  
  if ( startDir[0] != '\0' )
    sprintf(buf, "%s\n", startDir);
  
  else
    sprintf(buf, "FAILURE\n");
  
  /*write success*/
  Writen(new_sock, buf, strlen(buf)*sizeof(char));       
  
  /*get confirm*/
  ReadLine(new_sock, buf, MAXSIZE);

  if ( buf[0] != '?' )
    return -1;
  
  /*write '.'*/
  sprintf(buf, ".\n");
  Writen(new_sock, buf, strlen(buf) * sizeof(char));                  
  buf[0] = ' ';
  
  return 1;
}

long hdl_seek_informix(int new_sock) 
{
  int len = 0, res = 0;

 memset(buf, 0, MAXSIZE * sizeof(char));
 
 while( (len = ReadLine(new_sock, buf, MAXSIZE)) > 0 ) { /*read client cmd */
   
   if ( strncasecmp("info", buf, 4) == 0 ) {
     res = handleSeekInformix(new_sock);

     if ( res == -1 )
       break;
   }
   
   if ( buf[0]== '.' )
     break;
 }
 
 return 1;
}

int handleSeekInformix(int new_sock) 
{
  int res = 2;
  int len = 0;
  
  while ( res == 2 ) {

    memset(buf, 0, MAXSIZE * sizeof(char));
    res = getFileData();

    if ( res > 0 ) {
      Writen(new_sock, buf, strlen(buf) * sizeof(char));
      
      if ( (len = ReadLine(new_sock, buf, MAXSIZE)) > 0 ) {
	if ( strncasecmp("??", buf, 2) == 0 )
	  continue;
	else
	  break;
      }
    }
  }
  
  if ( res > 0 ) {
    sprintf(buf, "!!\n");
    Writen(new_sock, buf, strlen(buf) * sizeof(char));
  }
  
  return 1;
}

int getFileData()
{
  int res = 0;
  static int cnt = 0;
  
  static FILE *fp = NULL;
  char ch, data[32768];
  
  res = system("onstat -g ses >/tmp/ids_ses.txt");
  
  if ( fp == NULL )
    if ( (fp = fopen("/tmp/ids_ses.txt", "rb")) == NULL ) {
      sprintf(data, "Cannot open file.\n");
      return -1;
    }
  
  while ( ! feof(fp) ) {
    ch = getc(fp);
    
    if ( ferror(fp) ) {
      sprintf(data, "Read Error.\n");
      clearerr(fp);
      break;
    }
    
    else {
      
      if ( ! feof(fp) ) {
	
	if ( cnt < MAXSIZE - 2 ) {
	  cnt++;
	  sprintf(buf,"%s%c", buf, ch);
	}
	
	else {
	  sprintf(buf,"%s%c", buf, ch);
	  cnt = 0;
	  return 2;
	} 
      }
      
    } /*first else*/
  } /*while*/
  
  fclose(fp);
  
  res = system("rm -f /tmp/ids_ses.txt");
  
  return 1;
}

int getFiles(char *startDir, char buf[][MAXSIZE], 
	     int *rows, char *fltr) 
{
  struct dirent *curDir;
  
  DIR *dp, *testDir;
  int incDir = 0;
  int cnt = 0;
  int bufRow = 0;
  int dirFlg = 0;
  char testPath[256];
  char fileName[FILESIZE];
  char *exten;
  char *Msg;

  chdir(startDir);

  if ( (dp = opendir(startDir)) == (DIR *) NULL) {
    sprintf(ErrMsg, "getFiles(): Cannot open directory '%s'", startDir);
    
    if ( errno > 0 ) {
      Msg = strerror(errno);
      sprintf(ErrMsg, "%s %s", ErrMsg, Msg);
    } 
    return -1;
  }
  
  chdir(startDir);
  
  while ( (curDir = readdir(dp)) != NULL ) {
    incDir = 1;
    dirFlg = 0;
    
    /*    if (curDir->d_name[0]=='.') 
	  incDir = 0;*/
    
    sprintf(testPath, "%s%s", startDir, curDir->d_name);
    
    if ( (testDir = opendir(testPath)) != (DIR *) NULL ) {
      dirFlg = 1;
      closedir(testDir);
    }
    
    /*handle filters*/
    if ( fltr[0] != '\0' && 
	 strcmp(".", curDir->d_name) != 0 && 
	 strcmp("..", curDir->d_name) != 0 &&
	 dirFlg == 0 ) {
      
      strncpy(fileName, curDir->d_name, FILESIZE);
      strtok(fileName, ".");
      exten = strtok('\0', ".");
      
      if ( exten != NULL ) {
	if ( strcmp(fltr, exten) !=0 )
	  incDir = 0;
      }
      else
	incDir = 0;
    }
    
    if ( incDir ) {

      cnt = strlen(curDir->d_name) + 2 + strlen(buf[bufRow]);

      if ( cnt >= MAXSIZE && bufRow < ROWS - 1) {
	cnt = 1;
	sprintf(buf[bufRow], "%s\n", buf[bufRow]);
	bufRow++;
      }
      
      if ( cnt < MAXSIZE ) {
	
	if ( dirFlg )
	  sprintf(buf[bufRow], "%s%s/ ", buf[bufRow], curDir->d_name);
	else 
	  sprintf(buf[bufRow], "%s%s ", buf[bufRow], curDir->d_name);
      }
    }
  }
  
  sprintf(buf[bufRow], "%s\n", buf[bufRow]);
  *rows = bufRow;
  
  closedir(dp);
  
  return cnt;
}

int getFltr(char buf[], char *fltr) 
{
  char *chrpnt;
  char *st;
  int n;
  n = strlen(buf);
  st = buf;
  
  for ( chrpnt = buf; (chrpnt = strtok(chrpnt, " \t\r\n")) != NULL; 
	chrpnt = NULL) 
    
    if ( strncasecmp("ls", chrpnt, 2) != 0 ) {
      while ( *chrpnt != '.' && st + n >= chrpnt )
	chrpnt++;
      
      chrpnt++;
      
      if ( st + n <= chrpnt )
	strcpy(fltr, "\0");
      else
	strcpy(fltr, chrpnt);
      
      break;
    }
  
  return 1;
}

long hdl_kill_informix(int new_sock) 
{
  int len = 0, res = 0;
  
  memset(buf, 0, MAXSIZE * sizeof(char));
  
  while ( (len = ReadLine(new_sock, buf, MAXSIZE)) > 0 ) { /*read client cmd */
    if ( buf[0] != '.' ) {
      res = handleKillInformix(new_sock);

      if ( res == -1 )
	break;
    }
    else
      break;
  }
  
  return 1;
}

int handleKillInformix(int new_sock) 
{
  int res = 2;
  char *p;
  int i = 0;
  long code = 0, sid = 0, curSes = 0;
  char username[9];
  char *Msg;
 
/*split one line of buffer to code, user, current session, sid to be killed*/ 
  p = strtok(buf,"+");
  
  if ( p )
    code = atoi(p);

  i = 1;
  
  do {
    p = strtok('\0', "+");

    if ( p ) {

      switch ( i ) {

      case 0:  
	code = atol(p);
	break;

      case 1:
	strncpy(username, p, 8);
	username[8] = '\0';
	break;
	
      case 2:
	curSes = atol(p);
	break;

      case 3:
	sid = atol(p);
	break;
      }
      i++;   
    } 
  } while(p);
  
  /*DB DATA*/
  if ( res > 0 )
    res = connectDB("sysmaster");

  /*make sure current session belongs to provided username*/
  if ( res > 0 )
    res = sessionExistsServer(curSes, username);

  /*make sure session to be killed belongs to same user as above*/
  if ( res > 0 )
    res = sessionExistsServer(sid, username);
  
  if ( res > 0 )
    res = disconnectDB();
  /*END DB DATA*/
  
  if ( res < 1 )
    return -1;
  
  if ( res > 0 ) {
    sprintf(buf,"onmode -z %ld", sid);
    res = system(buf);
    
    if ( res < 0 )
      sprintf(buf, "FAILURE\n");
    else
      sprintf(buf,"SUCCESS\n");
    
    if ( errno > 0 ) {
      Msg = strerror(errno);
      sprintf(buf, "FAILURE %s\n", Msg);
    } 
    
    Writen(new_sock, buf, strlen(buf) * sizeof(char));
  }
  
  return 1;
}


int getPWD(char *startDir, char buf[]) 
{
  char *chrpnt;
  char testDir[512];
  char *sysDir;
  char *Msg;
  
  DIR *dp;
  
  strcpy(testDir, startDir);
  
  for (chrpnt = buf; (chrpnt = strtok(chrpnt, " \t\r\n")) != NULL; 
       chrpnt = NULL)
    
    if ( strncasecmp("cd", chrpnt, 2) !=0 ) {

      if ( chrpnt[0] == '/')
	sprintf(testDir,"%s", chrpnt);

      else
	sprintf(testDir, "%s%s/", testDir, chrpnt);
    }
  
  if ( (dp = opendir(testDir)) == (DIR *) NULL ) {
    sprintf(ErrMsg, "getPWD(): Cannot open directory '%s'", testDir);
    
    if ( errno > 0 ) {
      Msg = strerror(errno);
      sprintf(ErrMsg, "%s %s", ErrMsg, Msg);
    }
    
    return -1;
  }
  
  else {
    chdir(testDir);    
    sysDir = getcwd(NULL, 512);
    
    if (sysDir != NULL ) {
      
      sprintf(sysDir, "%s//", sysDir);
      memset(startDir, '\0', sizeof(startDir));
      strcpy(startDir, sysDir);
      sprintf(startDir, "%s", startDir);
    }
  }
  
  return 1;
}

long hdl_kill_idsserver(int new_sock) 
{
  int len = 0, res = 0;

  memset(buf, 0, MAXSIZE * sizeof(char));
  
  while( (len = ReadLine(new_sock, buf, MAXSIZE)) > 0 ) { /*read client cmd */
    
    if ( buf[0] != '.' ) {
      res = handleKillIdsserver(new_sock);
      if ( res == -1 )
	break;
    }
    else
      break;
  }
  
  return 1;
}

int handleKillIdsserver(int new_sock) 
{
  int res = 2;
  char *p;
  int i = 0;
  long code = 0, curSes = 0;
  char username[9];
  char *Msg;
  
  /*split one line of buffer to code, user, current session, sid to be killed*/ 
  p = strtok(buf,"+");

  if ( p )
    code = atoi(p);

  i = 1;
  
  do {
    p = strtok('\0', "+");

    if ( p ) {
      
      switch ( i ) {
      
      case 0:  
	code = atol(p);
	break;
	
      case 1:
	strncpy(username, p, 8);
	username[8] = '\0';
	break;
	
      case 2:
	curSes = atol(p);
	break;
      }
      i++;   
    } 
  } while(p);
  
  /*DB DATA*/
  if ( res > 0 )
    res = connectDB("sysmaster");
  
  /*make sure current session belongs to provided username*/
  if ( res > 0 )
    res = sessionExistsServer(curSes, username);

  /*make sure is dba on sysmaster on informix*/
  if ( res > 0 && 
       strncasecmp(username, "idsdb", 5) !=0 ) {
    res = isDBA(username);
    
    if ( res < 1 ) {
      sprintf(ErrMsg,"%s does not have the privileges to execute this tool.",
	      username);
      
      log_warning(ErrMsg);
      return -1;
    }
  }
  
  if ( res > 0 )
    res = disconnectDB();
  /*END DB DATA*/
  
  if ( res < 0 )
    sprintf(buf, "FAILURE\n");
  else
    sprintf(buf, "SUCCESS\n");
  
  if ( errno > 0 ) {
    Msg = strerror(errno);
    sprintf(buf, "FAILURE %s\n", Msg);
  } 
  
  Writen(new_sock, buf, strlen(buf) * sizeof(char));
  
  if ( res < 1 )
    return -1;
  
  /*KILL SHIT*/
  if ( res > 0 ) {
    
    kill(getppid(),SIGTSTP);              /* Kill IDS Server */
    sigpause(SIGTERM);
    
    return 1;
  }
  
  return 1;
}
E 1
