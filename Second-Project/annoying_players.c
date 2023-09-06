#include "local.h"

#define File "info_annoy.txt"

int endPoint, 
    speed,
    size_of_cup=(int)(BagSize/5);
    
/* Functions: */
void read_from_file();

    
/* Main: */
int main(int argc, char *argv[]){
    
    #ifdef __PRINT2__
    printf("Size of cup %d\n",size_of_cup);
    fflush(stdout);
    #endif
    
    
    key_t   key_P1,
            Key_another_annoy;
    
    
    int ppid= getppid();
    struct MEMORY *memptr;
    char  *shmptr;
    int shmid, amount_tank_A ,semidA, semid_P1, semid_P4,semanother;
    
    int player_team= atoi(argv[1]),   
        player_num= atoi(argv[2]),
        team_index = player_team-1,   
        player_index =player_num-1;
        
    int water_in_cup,    water_in_bag, 
        sand_in_bag=0,   p1_tr,
        distance=0,      min_range=0,
        max_range=0,     t_distance;
        
    float ratio_sand_in_bag;
    
    read_from_file(); 
    int initial_speed = speed;
    
     /* it move x for every 1 time, where x is speed  */
    t_distance = endPoint/ speed; /* traveled distance in each step*/ 
    WHITE;
    printf("> [ P%d%d ] : speed: %d \n\n",player_team,player_num, speed);
    fflush(stdout);
    

    switch(player_num){
          case 2:
                key_P1= K_loc_P1_P2;
                Key_another_annoy = K_loc_P3;        
          break;         
          case 3:
           
                key_P1= K_loc_P1_P3;
                Key_another_annoy = K_loc_P2;            
          break;
     }
    
    
    /*
     * Access, attach and reference the shared memory
     */
    if ( (shmid = shmget((int) ppid, 0, 0)) != -1 ) {
         if ( (shmptr = (char *) shmat(shmid, (char *)0, 0)) == (char *) -1 ) {
                perror("shmat -- annoying players -- attach");
                exit(1);
         }
         memptr = (struct MEMORY *) shmptr;
    }
    else {
          perror("shmget -- annoying players -- access");
          exit(2);
    }
       
    /*
     * Access the semaphore set Tank A
     */
    if ( (semidA = semget(K_tankA, 2, 0)) == -1 ) {
           perror("semget Tank A --  annoying players -- access");
           exit(3);
    }
    
    /*
     * Access the semaphore set P1
     */
    if ( (semid_P1 = semget(key_P1, 2, 0)) == -1 ) {
           perror("semget P1 --  annoying players -- access");
           exit(4);
    }
    
    /*
     * Access the semaphore set P4
     */
    if ( (semid_P4 = semget(K_loc_P4, 2, 0)) == -1 ) {
           perror("semget P1 --  annoying players -- access");
           exit(5);
    }
    
    /*
     * Access the semaphore set P1
     */
    if ( ( semanother= semget(Key_another_annoy, 2, 0)) == -1 ) {
           perror("semget P1 --  annoying players -- access");
           exit(6);
    }
    
    
      
     /* Set location of annoying player to 0
       *******************************/
       
       /* Block P4 from reading */
       acquire.sem_num = READ;
       if ( semop(semid_P4, &acquire, 1) == -1 ) {
	       perror("semop -- annoy P : P4  -- acquire");
	       exit(7);
       } 
       /* Set write to annoying P */
       release.sem_num = WRITE;
       if ( semop(semid_P4, &release, 1) == -1 ) {
	     perror("semop -- annoy P : P4  -- release");
	     exit(8);
       }

  
       /*Change: */ 
       memptr-> location[team_index][player_index] = 0 ;
       distance = memptr-> location[team_index][player_index];
       
     
       /* Access Read To P4 */
       acquire.sem_num = WRITE;
       if ( semop(semid_P4, &acquire, 1) == -1 ) {
	      perror("semop -- annoy P : P4   -- acquire");
	      exit(9);
       }

       /* Set read to P4 */
       release.sem_num= READ;
       if ( semop(semid_P4, &release, 1) == -1 ) {
	      perror("semop -- annoy P : P4  -- release");
	      exit(10);
       }
 
     
     /* Read the amount of water in tank A 
     =======================================*/
    acquire.sem_num = READ;
    if ( semop(semidA, &acquire, 1) == -1 ) {
          perror("semop -- annoying players -- acquire");
         exit(11);
     }
   
     amount_tank_A = memptr-> Amount_Tank_A ;
     release.sem_num = READ;
     if ( semop(semidA, &release, 1) == -1 ) {
          perror("semop -- annoying players -- acquire");
         exit(12);
     }
 
      
      
      

  /****************** Start ***********************/
    int return_to, check_add_sand=0 ;
    while(amount_tank_A > 0){
    	
    /* speed of player depends on the sand in player bag
    =======================================================*/
    
       /* Block P4 from reading */
        acquire.sem_num = READ;
        if ( semop(semid_P4, &acquire, 1) == -1 ) {
              perror("semop -- annoying players -- acquire");
              exit(13);
        }
   
        /* from team 1*/
        if(player_team ==1){
              if(player_num ==2){ // P12     
              	     sand_in_bag = memptr-> Sand_in_bag_P12;              	     
              }
              else if(player_num ==3){ // P13             
                    sand_in_bag = memptr-> Sand_in_bag_P13;              
              }
        }
        /* from team 2*/
        else{
              if(player_num ==2){ // P22             
              	     sand_in_bag = memptr->Sand_in_bag_P22;             
              }
              else if(player_num == 3 ){ // P23             
              	     sand_in_bag = memptr->Sand_in_bag_P23;              
              }       
        }
        
        release.sem_num = READ;
        if ( semop(semid_P4, &release, 1) == -1 ) {
              perror("semop -- annoying players -- acquire");
              exit(14);
        }
        
        
        
               /*
    	printf(" Sand in bag : %d\n", memptr->Sand_in_bag_P12);
    	fflush(stdout);
    	printf(" Sand in bag : %d\n", memptr->Sand_in_bag_P13);
    	fflush(stdout);
       */
        
        
        
     /* calculate speed ( will not be less than the half of initial speed )
     =======================================================================*/
     /* The ratio of sand in the bag to the size of bag*/
     ratio_sand_in_bag= sand_in_bag / (float)(Sand_BagSize) ;
     speed = (int)(initial_speed - (ratio_sand_in_bag * 0.5 * initial_speed ));
     
     #ifdef __PRINT3__
     YELLOW;
     printf(" ratio_sand_in_bag %.2f  speed %d  initial_speed %d sand_in_bag %d \n",ratio_sand_in_bag, speed,initial_speed, sand_in_bag);
     fflush(stdout);
     WHITE;
     #endif
	
	
   
        /* check if P1 within the speed range
          if yes, he will try to add steal the water
          if no, will move accorging to the site of P1  
        =================================================*/
    
          /* min range Should not be less than 0 */
	   if ( distance - speed  > 0 ){
	          min_range= distance - speed;
	   }
	   
	   /* max range should not be more than max distance between tanks */
	   if (distance +  speed < endPoint ){
	    
	        max_range= distance +  speed;
	   }
	   GREEN;
	   printf("[ P%d%d ]  distance:  %d  max:%d min:%d \n",player_team,player_num, distance, max_range,min_range);
	   fflush(stdout);
	   WHITE;
    
  
           /* find P1 location */
	    acquire.sem_num = READ;
	    if ( semop(semid_P1, &acquire, 1) == -1 ) {
		  perror("semop -- annoying players -- acquire");
		 exit(4);
	    }
	   
	    p1_tr= memptr-> location[0][0]; // must change to another team
	    printf("P1 location: %d \n",p1_tr);
	    fflush(stdout);
	     
	    release.sem_num = READ;
	    if ( semop(semid_P1, &release, 1) == -1 ) {
		  perror("semop -- annoying players -- acquire");
		 exit(4);
	    }
	    
	    
	    
	    /* In the case that annoying player can steal water from P1 one */
	    if( min_range <= p1_tr && p1_tr <= max_range && p1_tr != 0){
	          #ifdef __PRINT2__
	          printf("\ntry to steal\n\n");
	          fflush(stdout); 
	          #endif
	          
	          
	          /* check for P4 */
	         /*Block P1 from reading*/
	         #ifdef __CATCH__
	          acquire.sem_num = READ;
	          if ( semop(semid_P4, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }
	          
  	         /*Block another P  from reading
  	         =============================*/
	          acquire.sem_num = READ;
	          if ( semop(semanother, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }
	          /* Set write to annoying P */
	          release.sem_num = WRITE;
	          if ( semop(semid_P4,&release, 1) == -1 ) {
		        perror("semop -- P annoying -- release");
		        exit(4);
	          } 
	          /* Set write to annoying P */
	          release.sem_num = WRITE;
	          if ( semop(semanother,&release, 1) == -1 ) {
		        perror("semop -- P annoying -- release");
		        exit(4);
	          } 
	          
  	          check_add_sand = memptr-> P14_to_catch;
	          if (check_add_sand == team_index ){
	               memptr-> P14_to_catch = 3;
	               RED;
	               printf("   check_add_sand \n");
	               fflush(stdout);
	               WHITE;
	          }

	          //===================================
	          
	        /* Access reading on P4 */
	         acquire.sem_num = WRITE;
	         if ( semop(semid_P4, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }

	        /* Access reading  */
	         acquire.sem_num = WRITE;
	         if ( semop(semanother, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }
	          
	          release.sem_num = READ;
	          if ( semop(semid_P4, &release, 1) == -1 ) {
		         perror("semop -- P annoying-- release");
		         exit(4);
	          }
	          release.sem_num = READ;
	          if ( semop(semanother, &release, 1) == -1 ) {
		         perror("semop -- P annoying-- release");
		         exit(4);
	          }          
	          
	          #endif
	          
	          /* If he succeeds in stealing the water */
	          /* 1. sub. the amount of water from P1 bag
	             2. he will go back to Tank A 
	             3. he will add the water to tank A*/

	          /*Block P1 from reading*/
	          acquire.sem_num = READ;
	          if ( semop(semid_P1, &acquire, 1) == -1 ) {
		          perror("semop -- P1 -- acquire");
		          exit(4);
	          }
	          /* Set write to annoying P */
	          release.sem_num = WRITE;
	          if ( semop(semid_P1,&release, 1) == -1 ) {
		        perror("semop -- P1 -- release");
		        exit(4);
	          } 
	       
	         /* Change */
	         
	         water_in_bag = memptr-> water_in_bag_P1;
	         return_to=1;
	         if (water_in_bag >= size_of_cup ){
	              memptr-> water_in_bag_P1 -= size_of_cup;
	              water_in_cup= size_of_cup;
	              BLUE;
	              printf("  P%d%d Steal  %d of water from P1.\n\n",player_team,player_num,water_in_cup);
       	      fflush(stdout);
       	      WHITE;
       	      
	         }
	         else if(water_in_bag > 0 ){
	         	water_in_cup = memptr-> water_in_bag_P1;
	         	memptr-> water_in_bag_P1 = 0;
	         	BLUE;
	         	printf("  P%d%d Steal  %d of water from P1.\n\n",player_team,player_num,water_in_cup);
	         	fflush(stdout);
	         	WHITE;
	         	
	         }
	         else {
	               printf("The bag is empty!!\n");
	               return_to=0;
	         }
	         
	        /* Access reading on P1 */
	         acquire.sem_num = WRITE;
	         if ( semop(semid_P1, &acquire, 1) == -1 ) {
		          perror("semop -- P1 -- acquire");
		          exit(4);
	          }
	       
	          release.sem_num = READ;
	          if ( semop(semid_P1, &release, 1) == -1 ) {
		         perror("semop -- P1 : P1_P3 -- release");
		         exit(4);
	          }
	          
	          /* return the water to Tank A
	            1. Move to Tank A
	            2. Add water from cup to Tank A
	          ================================*/
	        if(return_to){
	        	printf("Return water to Tank A \n");
	        	fflush(stdout);
	        	while( distance > 0){
	        	
	             /* Change the location of Annoying player */
		       acquire.sem_num = READ;
		       if ( semop(semid_P4, &acquire, 1) == -1 ) {
			       perror("semop  P4 -- Annoying player -- acquire");
			       exit(4);
		       }
		       release.sem_num = WRITE;
		       if ( semop(semid_P4, &release, 1) == -1 ) {
			     perror("semop P4 -- Annoying player -- release");
			     exit(4);
		       }
		  
		       /*Change: */ 
		       memptr-> location[team_index][player_index ] -= speed ;
		       distance = memptr-> location[team_index][player_index ];
		       if(memptr-> location[team_index][player_index ] < 0){
		              memptr-> location[team_index][player_index ]=0;
		       }
		       if(distance <= 0){
		              distance = 0;
		       }
		       
		     
		       acquire.sem_num = WRITE;
		       if ( semop(semid_P4, &acquire, 1) == -1 ) {
			      perror("semop P4 -- Annoying player -- acquire");
			      exit(4);
		       }

		       
		       /* Set read to P2 & P3 */
		       release.sem_num= READ;
		       if ( semop(semid_P4, &release, 1) == -1 ) {
			      perror("semop P4 -- Annoying player -- release");
			      exit(4);
		       } 
		       if(distance <= 0){
		              distance = 0;
		       }
		       printf("distance:  %d  ",distance); 
		       fflush(stdout);
		       sleep(3);
		       
	              }
	              printf("\n");
	              fflush(stdout);
	              
	              /* Add water to Tank A 
	              =======================*/
	              
	              /* Block reading on Tank A */
       		acquire.sem_num = READ;
       		if ( semop(semidA, &acquire, 1) == -1 ) {
        		       perror("semop A -- Annoying player -- acquire");
           		       exit(4);
      			}

      		       /* Block another annoy from reading */
      		       //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
       		acquire.sem_num = READ;
       		if ( semop(semanother, &acquire, 1) == -1 ) {
			       perror("semop -- another annoy  -- acquire");
			       exit(4);
    		       }
    		       
    		       /* Set write to Tank A */
		       release.sem_num = WRITE;
		       if ( semop(semidA, &release, 1) == -1 ) {
			     perror("semop A -- Annoying player -- release");
			     exit(4);
		       }
		       
		      /* Set write to another annoy */
		       release.sem_num = WRITE;
		       printf("release.sem_num  : %d \n",release.sem_num );
		       if ( semop(semanother, &release, 1) == -1 ) {
			     perror("semop -- another annoy -- release");
			     exit(4);
			}
			
			/* Change */
			if (memptr-> Amount_Tank_A<=0){
			
			water_in_cup=0;
			}
			memptr-> Amount_Tank_A -= water_in_cup;
			RED;
			if (memptr-> Amount_Tank_A<=0){
			memptr-> Amount_Tank_A==0;
			}
			
			printf(" Amount Tank A after steal: %d\n",memptr-> Amount_Tank_A);
	               fflush(stdout);
	               WHITE;
			
		      /*Access Read from Tank A */
		       acquire.sem_num = WRITE;
		       if ( semop(semidA, &acquire, 1) == -1 ) {
			      perror("semop -- P1 -- acquire");
			      exit(4);
		       }
		       release.sem_num= READ;
		       if ( semop(semidA, &release, 1) == -1 ) {
			      perror("semop -- P1 -- release");
			      exit(4);
		       }
		       
		       
		       /*Access Read from Tank A */
		       acquire.sem_num = WRITE;
		       if ( semop(semanother, &acquire, 1) == -1 ) {
			      perror("semop -- another_annoy -- acquire");
			      exit(4);
		       }
		       release.sem_num= READ;
		       if ( semop(semanother, &release, 1) == -1 ) {
			      perror("semop -- another_annoy -- release");
			      exit(4);
		       }
			
	        
	       }
	       // ==========
	         
	          
	          
	          
	    }
	    
	    
	    
	    /* P1 ahead of Annoying player */
	   else if ( max_range < p1_tr ){
		printf("increase \n");
	
	       /* Change the location of Annoying player */
	       acquire.sem_num = READ;
	       if ( semop(semid_P4, &acquire, 1) == -1 ) {
		       perror("semop  P4 -- Annoying player -- acquire");
		       exit(4);
	       }
	       release.sem_num = WRITE;
	       if ( semop(semid_P4, &release, 1) == -1 ) {
		     perror("semop P4 -- Annoying player -- release");
		     exit(4);
	       }
	  
	       /*Change: */ 
	       memptr-> location[team_index][player_index ] += speed ;
	       distance = memptr-> location[team_index][player_index ];
	       if(memptr-> location[team_index][player_index ] < 0){
	              memptr-> location[team_index][player_index ]=0;
	       }
	       if(distance == 0){
	              distance = 0;
	       }
	     
	       acquire.sem_num = WRITE;
	       if ( semop(semid_P4, &acquire, 1) == -1 ) {
		      perror("semop P4 -- Annoying player -- acquire");
		      exit(4);
	       }

	       
	       /* Set read to P2 & P3 */
	       release.sem_num= READ;
	       if ( semop(semid_P4, &release, 1) == -1 ) {
		      perror("semop P4 -- Annoying player -- release");
		      exit(4);
	       }       
	    }
	    
	    /* Annoying player ahead of P1 */
	    else if(p1_tr < min_range){
		 printf("decrease\n");
		 
		 
	       /* Change the location of Annoying player */
	       acquire.sem_num = READ;
	       if ( semop(semid_P4, &acquire, 1) == -1 ) {
		       perror("semop  P4 -- Annoying player -- acquire");
		       exit(4);
	       }
	       release.sem_num = WRITE;
	       if ( semop(semid_P4, &release, 1) == -1 ) {
		     perror("semop P4 -- Annoying player -- release");
		     exit(4);
	       }
	  
	       /*Change: */ 
	       memptr-> location[team_index][player_index ] -= speed ;
	       distance = memptr-> location[team_index][player_index ];
	       if(memptr-> location[team_index][player_index ] < 0){
	              memptr-> location[team_index][player_index ]=0;
	       }
	       if(distance <= 0){
	              distance = 0;
	       }
	     
	       acquire.sem_num = WRITE;
	       if ( semop(semid_P4, &acquire, 1) == -1 ) {
		      perror("semop P4 -- Annoying player -- acquire");
		      exit(4);
	       }

	       
	       /* Set read to P2 & P3 */
	       release.sem_num= READ;
	       if ( semop(semid_P4, &release, 1) == -1 ) {
		      perror("semop P4 -- Annoying player -- release");
		      exit(4);
	       } 
	    }
	    
    
           
           sleep(3);
    
    
    
        /* Read the amount of water in tank A */
	    acquire.sem_num = READ;
	    if ( semop(semidA, &acquire, 1) == -1 ) {
		  perror("semop -- annoying players -- acquire");
		 exit(4);
	     }
	   
	     amount_tank_A = memptr-> Amount_Tank_A ;
	     release.sem_num = READ;
	     if ( semop(semidA, &release, 1) == -1 ) {
		  perror("semop -- annoying players -- acquire");
		 exit(4);
	     }
	     if (amount_tank_A<0){
	     amount_tank_A=0;
	     }
	      printf("Amount A : %d\n",amount_tank_A);
	      fflush(stdout);
      
    }
 
    
    
    
    
    

    return(0);
  
}


/* Functions: */
void read_from_file(){ 
     FILE * fp;
     fp = fopen(File, "r");
     if (fp == NULL) {
	  printf("Unable to open the file!! ");
	  fflush(stdout);
          exit(1);
      }
      
      char line[10];
      int len , i=0;
      int lin_arr[3];
      
      while(fgets(line,sizeof(line),fp)){
	   len = strlen(line); // get length of line
	   if( line[len-1] == '\n' ) line[len-1] = 0; // remove newline from a string 
	   len = strlen(line);
	          
	   /*remove white space or tab from the end of line*/
	   while (line[len-1] == ' ' ||  line[len-1] =='\t') {
	        line[len-1] = 0;
	        len--;
	   }     
	   lin_arr[i] = atoi(line);
	   i++;   
      }
      
      endPoint= lin_arr[0];
      speed= lin_arr[1];
      
      fclose(fp);
        
}



























