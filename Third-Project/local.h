#ifndef __LOCAL_H__
#define __LOCAL_H__

/* Colors */

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define WHITE  "\033[0;37m"
#define CYAN   "\x1b[36m"

/* To Include */
#include <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>


/* Variable */
#define NUMBER_OF_PRODUCTION_LINE 5
#define NUMBER_OF_TECH_EMPLOYEE  10  
#define NUMB_DATA_USER_DEFINED 17
#define NUMBER_OF_PRODUCT_TRUCKS 1
#define starage_limit_A 300
#define starage_limit_B 50
#define starage_Max_S2 4
#define L2_carried_quantity 15
#define material_first_technical_nedded_from_A 2
#define material_first_technical_nedded_from_B 1
#define Max_speed 7
#define Min_speed 3
#define Size_Of_Box 3
#define TRUCK_PRODUCT_SIZE 10


int amount_of_A_product_in_S,amount_of_B_product_in_S;
int number_needed_from_A,number_needed_from_B;  

/* structure */
struct Storage_S1{
  int quantity_of_A_product_in_S,
      quantity_of_B_product_in_S,
      number_needed_from_A,
      number_needed_from_B,
      number_of_truck_nedded_A,
      number_of_truck_nedded_B,
      trucks_departing_B, 
      move_A, // 1: If L1 moving unloaded material A   , 0: If not 
      move_B; // 1: If L1 moving unloaded material B   , 0: If not  
  
};

struct Storage_S2{
    int number_of_boxes;

};



struct Truck_A{

     int trucks_departing,
         quantity_arrived;
};

struct Truck_B{

     int trucks_departing,
         quantity_arrived;
};

struct Truck_Product{

    int truck_avalabile [NUMBER_OF_PRODUCT_TRUCKS];
    int number_of_boxes;

    int counter;/* truck on the turn */
         
};


struct Manufacturing_Lines{

    int A_in_line[NUMBER_OF_PRODUCTION_LINE],
         B_in_line[NUMBER_OF_PRODUCTION_LINE],
         conter_lines_A,
         conter_last_step;


    int manufac_product[NUMBER_OF_PRODUCTION_LINE],
        manufac_boxes[NUMBER_OF_PRODUCTION_LINE];
     
    
         
         
};

struct Production_Line{
    int line_employee[NUMBER_OF_TECH_EMPLOYEE];
    int counter;
};

struct tecnical_employee{
    int id,
        time,
        line;

};





#endif
































