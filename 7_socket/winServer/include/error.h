/* **************************************************************************
 * Copywrite (c)  Innovative Data Solutions Inc. 2001 All Rights Reserved
 *                No part of this program may be photocopied, reproduced,
 *                or translated to another programming language without the
 *                prior written consent of Innovative Data Solutions Inc.
 ************************************************************************** */
 
/*------------------------- File Header -----------------------------------
 *
 * File Name: error.h
 *
 * Description:
 *
 * Sccs Id:    @(#)error.h	1.1 08/16/01 20:03:23
 *
 * Related Files/Documents:
 *
 *
 * Rpgision History
 * ________________
 * Date      Author           Description
 *
 * 08-19-96  B. Almasri       Initial Version
 -------------------------------------------------------------------------*/

#ifndef _ERROR_H_
#define _ERROR_H_

#define ERR_FAILURE 0
#define ERR_SUCCESS 1

void print_date(unsigned long  *curr_date);
void print_time(unsigned long  *curr_time);

void log_error(char *msg);
void log_warning(char *msg);

int init_error_log(char *err_file_name);
int reopen_error_log(char *err_file_name);

void close_error_log();

int get_num_err();
int get_num_war();

#endif


