
/*
 * local header file 
 */

#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>


#include <unistd.h>
#include <limits.h>
#include <time.h> 
#include <math.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>


#define PUBLIC "/tmp/PUBLIC"
#define PID_solider "PID_soldier.txt"
#define B_SIZ (PIPE_BUF / 2)

#define MAX_SOLIDER 10
#define serverfifo "serverfifo"
#define File "info.txt"


/* Colors */
#define RED    printf("\033[0;31m");
#define GREEN  printf("\033[0;32m");
#define YELLOW printf("\033[0;33m");
#define BLUE   printf("\033[0;34m");
#define PURPLE printf("\033[0;35m");
#define CYAN  printf("\033[0;36m");
#define WHITE  printf("\033[0;37m");



struct message {
  char    fifo_name[B_SIZ];
  char    cmd_line[B_SIZ];
};


#endif

