#ifndef __LOCAL_H__
#define __LOCAL_H__

/* Coloring Codes */

#define RED    printf("\033[0;31m");
#define GREEN  printf("\033[0;32m");
#define YELLOW printf("\033[0;33m");
#define BLUE   printf("\033[0;34m");
#define WHITE  printf("\033[0;37m");


/* Required includes */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>



//****

#define time_round 90
#define Distance_between_tanks 40
#define TSizeA 50 
#define TSizeB 50  
#define BagSize 10 
#define Sand_BagSize 30 


#define player1 1L
#define sand_player 1L
#define annoying_player 1L

/* keys: */
#define K_tankA 20
#define K_tankB 30
#define K_tankC 40
#define K_tankD 50 

#define K_loc_P1_P2 60
#define K_loc_P1_P3 70
#define K_loc_P2 80
#define K_loc_P3 90
#define K_loc_P4 100


#define K_loc_P21_P2 110
#define K_loc_P21_P3 120
#define K_loc_P22 130
#define K_loc_P23 140
#define K_loc_P24 150


union semun {
  int              val;
  struct semid_ds *buf;
  ushort          *array; 
};

struct MEMORY {
  
  int Amount_Tank_A; /* [1]. A -------->  B */
  int Amount_Tank_B;
  int Amount_Tank_C;/* [2]. C --------->  D */
  int Amount_Tank_D;
  
  int water_in_bag_P1;
  int water_in_bag_P2;
  
  int Sand_in_bag_P12;
  int Sand_in_bag_P13;
  int Sand_in_bag_P22;
  int Sand_in_bag_P23;
  int stop;
  
  int P14_to_catch; /* 0 : no one, 1: P2, 2: P3 , 3: failed*/
  int P24_to_catch;
  
  
  
  int location[2][4]; /*p11 --> [0][0] | p21 --> [1][0]
                        p12 --> [0][1] | p22 --> [1][1]
                        p13 --> [0][2] | p23 --> [1][2]
                        p14 --> [0][3] | p24 --> [1][3] 
                        */
};


struct sembuf acquire = {0, -1, SEM_UNDO}, 
              release = {0,  1, SEM_UNDO};

enum { WRITE , READ };



#endif


