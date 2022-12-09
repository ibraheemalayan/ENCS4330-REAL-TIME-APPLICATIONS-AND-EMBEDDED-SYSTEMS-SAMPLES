/* ************************* File Header **************************
 *
 * File Name: error.c
 *
 * Revision History 
 * ________________
 * Date      Author           Description
 *
 * Mar-27-2005	Hanna Bullata, PhD
 * 
 *************************************************************** */
 
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "error.h"

static int num_of_errors;
static int num_of_warnings;
static FILE *err_file;


/***************************** log_warning() *******************************

    If the maximum number of warnings has not been exceeded
    "msg" is added to the warning log, and the number of 
    warnings is incremented

**************************************************************************/

void log_warning(char *msg)
{
  unsigned long curr_time;

  if ( err_file != NULL ) {
    ++num_of_warnings;
    curr_time = time(NULL);
    
    fprintf(err_file, "W\t");
    print_date(&curr_time);  /* output the date of the warning */
    print_time(&curr_time);  /* output the time of the warning */
    fprintf(err_file, "%s\n",msg);      /* output warning message */
  }
}


/*************************** log_error() *******************************
 
        If the maximum number of errors has not been exceeded
        "msg" is added to the error log, and the number of 
        errors is incremented
 
***************************************************************************/

void log_error(char *msg)
{
  unsigned long curr_time;
  
  if ( err_file != NULL ) {
    ++num_of_errors;
    curr_time = time(NULL);
    
    fprintf(err_file, "E\t");
    print_date(&curr_time);  /* output the date of the error */
    print_time(&curr_time);  /* output the time of the error */
    fprintf(err_file, "%s\n",msg);      /* output error message */
  }
}


/*
 * init_error_log(): Initialization of error and warning logs
 */

int init_error_log(char *err_file_name)
{
  num_of_errors = 0;
  num_of_warnings = 0;
  
  if ( (err_file = fopen(err_file_name, "w")) == NULL ) {
    printf("Error - Could not open %s for writing.\n",
	   err_file_name);
    return ERR_FAILURE;
  }
  
  return ERR_SUCCESS;
}

/*
 * init_error_log(): Initialization of error and warning logs
 */

int reopen_error_log(char *err_file_name)
{
  int res = ERR_SUCCESS;
  
  if ( (err_file = fopen(err_file_name, "a")) == NULL ) {
    printf("Error - Could not open %s for writing.\n",err_file_name);
    res = ERR_FAILURE;
  }
  
  if ( res == ERR_FAILURE ) {
    printf("DAM EXIT HERE\n");
    exit (1);
  }
  
  return ERR_SUCCESS;
}    

/*
 * close_error_log(): Close of error and warning logs
 */

void close_error_log()
{
  
  if ( err_file != NULL ) {
    fprintf(err_file, "\nErrors = %d \tWarnings = %d\n",
	    num_of_errors,num_of_warnings);
    fclose(err_file);
  }
}

/*
 * print_date(): Outputs the current date to "err_file"
 */

void print_date(unsigned long  *curr_date)
{
  struct tm *start;
  static char ptr_to_date[40];
  
  start = localtime((time_t *)curr_date);
  
  strftime(ptr_to_date,40,"%m-%d-%y",start);
  fprintf(err_file,"%s\t",ptr_to_date);
}

/*
 * print_time(): Outputs the current time to the "err_file"
 */

void print_time(unsigned long  *curr_time)
{
  struct tm *start;
  static char ptr_to_time[40];
  
  start = localtime((time_t *)curr_time);
  strftime(ptr_to_time,40,"%H:%M:%S",start);
  fprintf(err_file,"%s\t",ptr_to_time);
}

/*
 * get_num_err():
 */

int get_num_err()
{
  return num_of_errors;
}

/*
 * get_num_war():
 */

int get_num_war()
{
  return num_of_warnings;
}



