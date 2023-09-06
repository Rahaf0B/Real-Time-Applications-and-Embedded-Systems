#include "local.h"
#define File "info_sand.txt"


int endPoint, 
    speed;
    
/* Functions: */
void read_from_file();
		
/* Main: */
int main(int argc, char *argv[]){

    
      
    int sand_in_bag_p4= Sand_BagSize, sand_in_bag_1, sand_in_bag_2;
    int size_of_sand_cup= (int)(Sand_BagSize/7);
    
    printf("size_of_sand_cup %d\n",size_of_sand_cup);
    fflush(stdout);
    
    
    int ppid= getppid();
    struct MEMORY *memptr;
    char  *shmptr;
    
    int shmid, semid_P2, semid_P3,  semid_P4, semidA, semid_P1_1, semid_P1_2;
    
    int t_distance, amount_tank_A;
    
     
    
    int     to_catch, 
            annoy,
            P4_to_catch_p=0,
            failed=1,
            check_add_sand;
    
    int player_team= atoi(argv[1]);
    int team_index = player_team-1;
    printf("> [P%d4]: sand \n",player_team);
    
    
    read_from_file(); 
    /* it move x for every 1 time, where x is speed  */
    t_distance = endPoint/ speed; /* traveled distance in each step*/ 
    printf("> P%d4: endPoint: %d, speed: %d, t_distance: %d \n\n",player_team, endPoint, speed, t_distance);
    fflush(stdout);
    
    
    
     /*
     * Access, attach and reference the shared memory
     */
    if ( (shmid = shmget((int) ppid, 0, 0)) != -1 ) {
         if ( (shmptr = (char *) shmat(shmid, (char *)0, 0)) == (char *) -1 ) {
                perror("shmat -- Sand player -- attach");
                exit(1);
         }
    memptr = (struct MEMORY *) shmptr;
    }
    else {
          perror("shmget -- Sand player -- access");
          exit(2);
    }  
    
    /*
     * Access the semaphore set P1 
     */
    if ( (semid_P1_1 = semget(K_loc_P1_P2, 2, 0)) == -1 ) {
           perror("semget --  Sand player -- access");
           exit(3);
    }
    
    /*
     * Access the semaphore set P1 
     */
    if ( (semid_P1_2 = semget(K_loc_P1_P3, 2, 0)) == -1 ) {
           perror("semget --  Sand player -- access");
           exit(3);
    }
    
    /*
     * Access the semaphore set P2 
     */
    if ( (semid_P2 = semget(K_loc_P2, 2, 0)) == -1 ) {
           perror("semget --  Sand player -- access");
           exit(3);
    }
    
    
    /*
     * Access the semaphore set P3
     */
    if ( (semid_P3 = semget(K_loc_P3, 2, 0)) == -1 ) {
           perror("semget  -- Sand player-- access");
           exit(3);
    }
    
    /*
     * Access the semaphore set P4
     */
    if ( (semid_P4 = semget(K_loc_P3, 2, 0)) == -1 ) {
           perror("semget  -- Sand player-- access");
           exit(3);
           
      }
      
  /*
   * Access the semaphore set Tank A
   */
  if ( (semidA = semget(K_tankA, 2, 0)) == -1 ) {
        perror("semget -- Sand player -- access");
        exit(3);
  }
  
      /* Read the locartion of P4 */
  acquire.sem_num = READ;
  if ( semop(semid_P4, &acquire, 1) == -1 ) {
        perror("semop -- Sand player -- acquire");
       exit(4);
   }
   
   int location = memptr-> location[team_index][3];
   release.sem_num = READ;
   
   if ( semop(semid_P4, &release, 1) == -1 ) {
        perror("semop -- Sand player -- acquire");
       exit(4);
   }
    printf("location P1%d: %d\n",player_team,location);
    
   /******/
   
    
    
     /* Read the amount of water in tank A 
     =======================================*/
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
      printf("Amount A : %d\n",amount_tank_A);
      
      int distance = 0, min_range=0, max_range=0 ;
      int p1_tr, p2_tr, p3_tr, choose=0;
      
      /****************** Start ***********************/
      while(amount_tank_A > 0){
      
      /* check if P2, P3 within the speed range 
      ===========================================*/
      /* min range Should not be less than 0 */
	   if ( distance - speed  > 0 ){
	          min_range= distance - speed;
	   }
	   
	   /* max range should not be more than max distance between tanks */
	   if (distance +  speed < endPoint ){
	    
	        max_range= distance +  speed;
	   }
	   if(distance <= 0){
	          distance = 0;
          }
	   YELLOW;
	   printf("[ P%d4 ]  distance:  %d  max:%d min:%d \n",player_team, distance, max_range,min_range);
	   fflush(stdout);
	   WHITE;
	   
	   
	   /* find P2, P3 locations */
	   
	   acquire.sem_num = READ;
	    if ( semop(semid_P2, &acquire, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	    
	    acquire.sem_num = READ;
	    if ( semop(semid_P3, &acquire, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	    
    	    acquire.sem_num = READ;
	    if ( semop(semid_P1_1, &acquire, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
    	    acquire.sem_num = READ;
	    if ( semop(semid_P1_2, &acquire, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	    
	    
	    /**/
	    
	    
	    p2_tr= memptr-> location[0][1]; // must change 
	    printf("P2 location: %d \n",p2_tr);
	    fflush(stdout);
	    
	    p3_tr= memptr-> location[0][2]; // must change 
	    printf("P3 location: %d \n",p3_tr);
	    fflush(stdout);
	    
    	    p1_tr= memptr-> location[0][0]; // must change 
	    printf("P3 location: %d \n",p3_tr);
	    fflush(stdout);
	    
     
	    release.sem_num = READ;
	    if ( semop(semid_P2, &release, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	   
      	    release.sem_num = READ;
	    if ( semop(semid_P3, &release, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	    release.sem_num = READ;
	    if ( semop(semid_P1_1, &release, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	    release.sem_num = READ;
	    if ( semop(semid_P1_2, &release, 1) == -1 ) {
		  perror("semop -- Sand player -- acquire");
		 exit(4);
	    }
	    
	    
	    
	    /* In the case that sand player can catch both P2, P3
	       he will chose one of them randomly 
	    */
	    
	    failed=1;
	       
	    if( min_range <= p2_tr && p2_tr <= max_range && p2_tr != 0 &&
	        min_range <= p3_tr && p3_tr <= max_range && p3_tr != 0){
	    	    GREEN;
	      	   printf("@ Try catch one of P2 or P3 \n");	
	      	   fflush(stdout);
	      	   WHITE;
	    	   	    	   
	    	   choose =rand() % 2;
		   if (choose){
		           YELLOW;
		      	   printf(" Try catch P2 \n");	
		      	   fflush(stdout);
		      	   WHITE;
		      	   to_catch = semid_P2;
		      	   annoy = semid_P3;
		      	   P4_to_catch_p=1;
		      	   
		   }else{
			   YELLOW;
		      	   printf(" Try catch P3 \n");	
		      	   fflush(stdout);
		      	   WHITE;
		      	   to_catch = semid_P3;
		      	   annoy = semid_P2;
		      	   P4_to_catch_p=2;
		   }
		   
		   #ifdef __CATCH__
		   
		   acquire.sem_num = READ;
	          if ( semop(semid_P4, &acquire, 1) == -1 ) {
		          perror("semop -- P Sand -- acquire");
		          exit(4);
	          }
	          acquire.sem_num = READ;
	          if ( semop(semid_P2, &acquire, 1) == -1 ) {
		          perror("semop -- P Sand -- acquire");
		          exit(4);
	          }
	          acquire.sem_num = READ;
	          if ( semop(semid_P3, &acquire, 1) == -1 ) {
		          perror("semop -- P Sand -- acquire");
		          exit(4);
	          }
	          
	        
	          release.sem_num = WRITE;
	          if ( semop(semid_P4,&release, 1) == -1 ) {
		        perror("semop -- P Sand -- release");
		        exit(4);
	          } 
  	          release.sem_num = WRITE;
	          if ( semop(semid_P2,&release, 1) == -1 ) {
		        perror("semop -- P Sand -- release");
		        exit(4);
	          } 
  	          release.sem_num = WRITE;
	          if ( semop(semid_P3,&release, 1) == -1 ) {
		        perror("semop -- P Sand -- release");
		        exit(4);
	          } 
	         /* Change */
	         memptr-> P14_to_catch = P4_to_catch_p;
                /**********/
	         acquire.sem_num = WRITE;
	         if ( semop(semid_P4, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }
	          
	         
	         acquire.sem_num = WRITE;
	         if ( semop(semid_P2, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }
	          
	         acquire.sem_num = WRITE;
	         if ( semop(semid_P3, &acquire, 1) == -1 ) {
		          perror("semop -- P annoying -- acquire");
		          exit(4);
	          }
	       
	          release.sem_num = READ;
	          if ( semop(semid_P4, &release, 1) == -1 ) {
		         perror("semop -- P annoying-- release");
		         exit(4);
	          }
	          
	          release.sem_num = READ;
	          if ( semop(semid_P2, &release, 1) == -1 ) {
		         perror("semop -- P annoying-- release");
		         exit(4);
	          }
	          
	          release.sem_num = READ;
	          if ( semop(semid_P3, &release, 1) == -1 ) {
		         perror("semop -- P annoying-- release");
		         exit(4);
	          }      
	          #endif

	          /* wait for responce */
	       
		   
		
		   
	 
	    }
	    
	    
	    /* In the case that sand player can catch P2 or P3*/ 
	    /* 1. catch P2  */
	    else if( min_range <= p2_tr && p2_tr <= max_range && p2_tr != 0){
	           to_catch = semid_P2;
	           YELLOW;
	      	   printf(" Try catch P2 only  \n");	
	      	   fflush(stdout);
	      	   WHITE;     
	      	   
   		 /*Block P2 from reading*/
	          acquire.sem_num = READ;
	          if ( semop(to_catch, &acquire, 1) == -1 ) {
		          perror("semop -- Sand player -- acquire");
		          exit(4);
	          }
	         /* Set write to annoying P */
	          release.sem_num = WRITE;
	          if ( semop(to_catch, &release, 1) == -1 ) {
		        perror("semop -- Sand player -- release");
		        exit(4);
	          } 
		   
		 /* Change */
           	  if ( sand_in_bag_p4 >= size_of_sand_cup ){
           	  	memptr->Sand_in_bag_P12 += size_of_sand_cup;  
           	  	sand_in_bag_p4 -= size_of_sand_cup;	
           	  }
           	  else if (sand_in_bag_p4 < size_of_sand_cup ){
           	        memptr->Sand_in_bag_P12 += sand_in_bag_p4 ;
           	        sand_in_bag_p4 = 0 ;          	        
           	  }           	  
           	  RED;
   	  	  printf(" sand in P2 bag: %d , after P4 add %d\n",
   	  	  memptr->Sand_in_bag_P12,size_of_sand_cup);	
   	  	    	
		  fflush(stdout);
		  WHITE;
		   
		   
		/* Access reading on P2 */
	         acquire.sem_num = WRITE;
	         if ( semop(to_catch, &acquire, 1) == -1 ) {
		          perror("semop -- Sand player -- acquire");
		          exit(4);
	          }	       
	          release.sem_num = READ;
	          if ( semop(to_catch, &release, 1) == -1 ) {
		         perror("semop -- Sand player -- release");
		         exit(4);
	          }   
	          
	          sleep(2);  	      
	    }
	    
	    /* 2. catch P3  */
	    else if( min_range <= p3_tr && p3_tr <= max_range && p3_tr != 0){
	           to_catch = semid_P3;
	           YELLOW;
	      	   printf(" Try catch P3 only  \n");	
	      	   fflush(stdout);
	      	   WHITE;  
	      	   
   		  /*Block P3 from reading*/
	          acquire.sem_num = READ;
	          if ( semop(to_catch, &acquire, 1) == -1 ) {
		          perror("semop -- Sand player -- acquire");
		          exit(4);
	          }
	         /* Set write to annoying P3 */
	          release.sem_num = WRITE;
	          if ( semop(to_catch, &release, 1) == -1 ) {
		        perror("semop -- Sand player -- release");
		        exit(4);
	          } 
		   
		 /* Change */ 
           	  if ( sand_in_bag_p4 >= size_of_sand_cup ){
           	  	memptr->Sand_in_bag_P13 += size_of_sand_cup;
           	  	sand_in_bag_p4 -=  size_of_sand_cup;
   	  	  }
           	  else if (sand_in_bag_p4 < size_of_sand_cup ){
           	        memptr->Sand_in_bag_P13 += sand_in_bag_p4 ;   
           	        sand_in_bag_p4 = 0 ;    	        
   	          }  	         
   	  	  RED;
   	  	  printf(" sand in P3 bag: %d , after P4 add %d\n",
   	  	  memptr->Sand_in_bag_P13,size_of_sand_cup);	           	  	
	          fflush(stdout);
	          WHITE;
	          
		/* Access reading on P3 */
	         acquire.sem_num = WRITE;
	         if ( semop(to_catch, &acquire, 1) == -1 ) {
		          perror("semop -- Sand player -- acquire");
		          exit(4);
	          }
	       
	          release.sem_num = READ;
	          if ( semop(to_catch, &release, 1) == -1 ) {
		         perror("semop -- Sand player -- release");
		         exit(4);
	          } 
	          sleep(2); 
	    }
	    /* if he can not catch any one 
	       will move close to P1 
	     ===========================*/
	    else {
	           
	           if ( max_range < p1_tr ){
	           	printf("increase \n");
	           	
	           	/* Change the location of Sand player */
	           	
	           	acquire.sem_num = READ;
		       if ( semop(semid_P2, &acquire, 1) == -1 ) {
			       perror("semop -- Sand player -- acquire");
			       exit(4);
		       }
		       
           	       acquire.sem_num = READ;
		       if ( semop(semid_P3, &acquire, 1) == -1 ) {
			       perror("semop -- Sand player -- acquire");
			       exit(4);
		       }
		       
		       release.sem_num = WRITE;
		       if ( semop(semid_P2, &release, 1) == -1 ) {
			     perror("semop -- Sand player -- release");
			     exit(4);
		       }
		       release.sem_num = WRITE;
		       if ( semop(semid_P3, &release, 1) == -1 ) {
			     perror("semop -- Sand player -- release");
			     exit(4);
		       }
	           	
	           	
	           	memptr-> location[team_index][4] += speed ;
	       	distance = memptr-> location[team_index][4];
	           	
	           	
   		       acquire.sem_num = WRITE;
		       if ( semop(semid_P2, &acquire, 1) == -1 ) {
			      perror("semop -- Sand player  -- acquire");
			      exit(4);
		       }
   		       acquire.sem_num = WRITE;
		       if ( semop(semid_P3, &acquire, 1) == -1 ) {
			      perror("semop -- Sand player  -- acquire");
			      exit(4);
		       }
		       
		       
		       release.sem_num= READ;
		       if ( semop(semid_P2, &release, 1) == -1 ) {
			      perror("semop -- Sand player  -- release");
			      exit(4);
		       } 	           	
		       release.sem_num= READ;
		       if ( semop(semid_P3, &release, 1) == -1 ) {
			      perror("semop -- Sand player  -- release");
			      exit(4);
		       } 
	
	           }
	           /* Annoying player ahead of P1 */
	    	    else if(p1_tr < min_range){
		    printf("decrease\n");
		    
	           	/* Change the location of Sand player */
	           	
	           	acquire.sem_num = READ;
		       if ( semop(semid_P2, &acquire, 1) == -1 ) {
			       perror("semop -- Sand player -- acquire");
			       exit(4);
		       }
		       
           	       acquire.sem_num = READ;
		       if ( semop(semid_P3, &acquire, 1) == -1 ) {
			       perror("semop -- Sand player -- acquire");
			       exit(4);
		       }
		       
		       release.sem_num = WRITE;
		       if ( semop(semid_P2, &release, 1) == -1 ) {
			     perror("semop -- Sand player -- release");
			     exit(4);
		       }
		       release.sem_num = WRITE;
		       if ( semop(semid_P3, &release, 1) == -1 ) {
			     perror("semop -- Sand player -- release");
			     exit(4);
		       }
	           	
	           	
	           	memptr-> location[team_index][4] -= speed ;
	       	distance = memptr-> location[team_index][4];
	           	
	           	
   		       acquire.sem_num = WRITE;
		       if ( semop(semid_P2, &acquire, 1) == -1 ) {
			      perror("semop -- Sand player  -- acquire");
			      exit(4);
		       }
   		       acquire.sem_num = WRITE;
		       if ( semop(semid_P3, &acquire, 1) == -1 ) {
			      perror("semop -- Sand player  -- acquire");
			      exit(4);
		       }
		       
		       
		       release.sem_num= READ;
		       if ( semop(semid_P2, &release, 1) == -1 ) {
			      perror("semop -- Sand player  -- release");
			      exit(4);
		       } 	           	
		       release.sem_num= READ;
		       if ( semop(semid_P3, &release, 1) == -1 ) {
			      perror("semop -- Sand player  -- release");
			      exit(4);
		       } 
		    

		    
		    }
	           
	           
	           

      
    	    }
	    sleep(2);  
	    
	    	           
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
		      printf("Amount A : %d\n",amount_tank_A);   
	           
	    
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


































