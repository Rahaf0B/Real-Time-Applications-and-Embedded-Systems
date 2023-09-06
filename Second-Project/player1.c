#include "local.h"


int start_tank;
int end_tank;

#define File "info_p1.txt"


int endPoint, 
    speed;


/* Functions: */
void read_from_file();


/* Main: */

int main(int argc, char *argv[]){

  

  int ppid= getppid();
  struct MEMORY *memptr;
  char  *shmptr;
  int shmid, amount_tank_from , amount_tank_to, semid_P1_P2, semid_P1_P3,
      semid_to, semid_from ;
  
  int  max, min, lost, distance=0,  
       t_distance,
       fell_on_ground = 0,
       player_team= atoi(argv[1]);
       
  int team_index= player_team -1;
  
  key_t  Key_from,
         Key_to,
         k_p1_p2,
         k_p1_p3;
  
  read_from_file(); 
  
  /* it move x for every 1 time, where x is speed  */
   t_distance = endPoint/ speed; /* traveled distance in each step*/ 
   #ifdef __PRINT1__
   printf("> [ P%d1 ] -> speed: %d \n\n",player_team, speed);
   fflush(stdout);
   #endif
   
   
   
      /* check team*/
      if(player_team == 1){
      	    Key_from = K_tankA;
      	    Key_to = K_tankB;
            
            
      }
      else if(player_team == 2){
      	     Key_from = K_tankC;
      	     Key_to = K_tankD;
      	     	     
      }
   

   /*
   * Access, attach and reference the shared memory
   */
  if ( (shmid = shmget((int) ppid, 0, 0)) != -1 ) {
    if ( (shmptr = (char *) shmat(shmid, (char *)0, 0)) == (char *) -1 ) {
          perror("shmat -- P1 -- attach");
          exit(1);
    }
    memptr = (struct MEMORY *) shmptr;
  }
  else {
        perror("shmget -- P1 -- access");
        exit(2);
  }
  
   /*
   * Access the semaphore set Tank A
   */
  if ( (semid_from = semget(Key_from, 2, 0)) == -1 ) {
        perror("semget -- P1 -- access");
        exit(3);
  }
  
   /*
   * Access the semaphore set Tank B
   */
  if ( (semid_to = semget(Key_to, 2, 0)) == -1 ) {
    perror("semget -- P1 -- access");
    exit(4);
  }
  
           /* check team*/
      if(player_team == 1){
             k_p1_p2= K_loc_P21_P2;
             k_p1_p3= K_loc_P21_P3;
                              
      }
      else if(player_team == 2){
      	     
      	     k_p1_p2= K_loc_P1_P2;
      	     k_p1_p3= K_loc_P1_P3;    
      }
  
   /*
   * Access the semaphore set P1 with P2
   */
  if ( (semid_P1_P2 = semget(k_p1_p2, 2, 0)) == -1 ) {
    perror("semget : P1_P2 --- P1 --- access");
    exit(5);
  }
  
   /*
   * Access the semaphore set P1 with P2
   */
  if ( (semid_P1_P3 = semget(k_p1_p3, 2, 0)) == -1 ) {
    perror("semget : P1_P3 --- P1 --- access");
    exit(6);
  }
  
  
/**************************************************************************/
 
  /* Read the locartion of P1 */
  acquire.sem_num = READ;
  if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
        perror("semop -- P1 : P1_P2 -- acquire");
       exit(7);
   }
   
   int location = memptr-> location[team_index][0];
   release.sem_num = READ;
   
   if ( semop(semid_P1_P2, &release, 1) == -1 ) {
        perror("semop -- P1: P1_P2 -- acquire");
       exit(8);
   }
   
   #ifdef __PRINT1__
    printf("location P%d1: %d\n",player_team,location);
    fflush(stdout);
   #endif
   /******/
  
  
  /* Read the amount of water in tank A */
  acquire.sem_num = READ;
  if ( semop(semid_from, &acquire, 1) == -1 ) {
        perror("semop -- P1 -- acquire");
       exit(9);
   }
   
   
   

         /* check team*/
      if(player_team == 1){
             amount_tank_from  = memptr-> Amount_Tank_A ;                   
      }
      else if(player_team == 2){
      	     amount_tank_from  = memptr-> Amount_Tank_C ;
      	     	     
      }
   
   
   
   release.sem_num = READ;
   if ( semop(semid_from, &release, 1) == -1 ) {
        perror("semop -- P1 -- acquire");
       exit(10);
   }
   #ifdef __PRINT1__
    printf("Amount: %d\n",amount_tank_from);
    fflush(stdout)
   #endif


  int fall_prob;
   
  while(amount_tank_from > 0){
       
      /* Set location of player 1 to 0
       *******************************/
       
       /* Block P2 from reading */
       acquire.sem_num = READ;
       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
	       perror("semop -- P1 : P1_P2  -- acquire");
	       exit(11);
       }
       /*Block P3 from reading*/
       acquire.sem_num = READ;
       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
	       perror("semop -- P1 : P1_P3  -- acquire");
	       exit(12);
       }
       
       /* Set write to P1 */
       release.sem_num = WRITE;
       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
	     perror("semop -- P1 : P1_P2 -- release");
	     exit(13);
       }
       release.sem_num = WRITE;
       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
	     perror("semop -- P1 : P1_P3 -- release");
	     exit(14);
       }
       
  
       /*Change: */ 
       memptr-> location[team_index][0] = 0 ;
       distance = memptr-> location[team_index][0];
       
     
       /* Access Read To P2 and P3  */
       acquire.sem_num = WRITE;
       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
	      perror("semop -- P1 : P1_P2  -- acquire");
	      exit(15);
       }
       acquire.sem_num = WRITE;
       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
	      perror("semop -- P1 : P1_P3  -- acquire");
	      exit(16);
       }
       
       /* Set read to P2 & P3 */
       release.sem_num= READ;
       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
	      perror("semop -- P1 : P1_P2 -- release");
	      exit(17);
       }
       release.sem_num= READ;
       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
	      perror("semop -- P1 : P1_P3 -- release");
	      exit(18);
       }
       
       /************************************************/
     
       fell_on_ground=0;

   
      /* Sub. the amount of warer P1 took: 
       *
       * Change amount of water in tank A
       =================================== */
       
 	/* Block reading on Tank A */
       acquire.sem_num = READ;
       if ( semop(semid_from, &acquire, 1) == -1 ) {
               perror("semop -- P1 -- acquire");
               exit(19);
       }

       /* Block P2 from reading */
       acquire.sem_num = READ;
       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
	       perror("semop -- P1 : P1_P2  -- acquire");
	       exit(20);
       }
       /*Block P3 from reading*/
       acquire.sem_num = READ;
       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
	       perror("semop -- P1 : P1_P3  -- acquire");
	       exit(21);
       }
       
       /* Set reading to Tank A */
       release.sem_num = WRITE;
       if ( semop(semid_from, &release, 1) == -1 ) {
             perror("semop -- P1 -- release");
             exit(22);
       }
       
       /* Set write to P1 */
       release.sem_num = WRITE;
       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
	     perror("semop -- P1 : P1_P2 -- release");
	     exit(23);
       }
       release.sem_num = WRITE;
       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
	     perror("semop -- P1 : P1_P3 -- release");
	     exit(24);
       }
       
       // ================ Change ===================
       /* check if the amount of water in tank A is big than or equale the size of bag*/
       
                /* check team*/
      if(player_team == 1){
              if (memptr-> Amount_Tank_A >= BagSize){
		      memptr-> Amount_Tank_A -= BagSize;
		      amount_tank_from = memptr-> Amount_Tank_A;
		      memptr-> water_in_bag_P1= BagSize;
		      #ifdef __PRINT1__
		      printf("  >> Take all \n");
		      fflush(stdout);
		      #endif
             }
             /* if less he will take all remaining quantity */
             else{
		     memptr-> water_in_bag_P1= memptr-> Amount_Tank_A;
		     memptr-> Amount_Tank_A = 0;
		     #ifdef __PRINT1__
		     RED;
		     printf("  >> Take remaining \n");
		     fflush(stdout); 
		     WHITE;   
		     #endif 
             }
                    
      }
      else if(player_team == 2){
           if (memptr-> Amount_Tank_C >= BagSize){
		      memptr-> Amount_Tank_C -= BagSize;
		      amount_tank_from = memptr-> Amount_Tank_C;
		      memptr-> water_in_bag_P1= BagSize;
		      #ifdef __PRINT1__
		      printf("  >> Take all \n");
		      fflush(stdout);
		      #endif
             }
             /* if less he will take all remaining quantity */
             else{
		     memptr-> water_in_bag_P1= memptr-> Amount_Tank_C;
		     memptr-> Amount_Tank_C = 0;
		     #ifdef __PRINT1__
		     RED;
		     printf("  >> Take remaining \n");
		     fflush(stdout); 
		     WHITE;   
		     #endif 
             }
      	        	     
      }
       
       

       
       /* Time needed to fill the bag */
       sleep(3);
       
       GREEN;
       printf(">>  P%d1  filled the bag with water \n", player_team );
       fflush(stdout);
       WHITE;
         
       #ifdef __PRINT1__ 
       printf("Amount (from) Tank  : %d\n",amount_tank_from);
       fflush(stdout);
       #endif
      // ================================================
       
       /*Access Read from Tank A */
       acquire.sem_num = WRITE;
       if ( semop(semid_from, &acquire, 1) == -1 ) {
              perror("semop -- P1 -- acquire");
              exit(25);
       } 
       /* Access Read To P2 and P3  */
       acquire.sem_num = WRITE;
       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
	      perror("semop -- P1 : P1_P2  -- acquire");
	      exit(26);
       }
       acquire.sem_num = WRITE;
       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
	      perror("semop -- P1 : P1_P3  -- acquire");
	      exit(27);
       }
       release.sem_num= READ;
       if ( semop(semid_from, &release, 1) == -1 ) {
              perror("semop -- P1 -- release");
              exit(28);
       }
       
       /* Set read to P2 & P3 */
       release.sem_num= READ;
       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
	      perror("semop -- P1 : P1_P2 -- release");
	      exit(29);
       }
       release.sem_num= READ;
       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
	      perror("semop -- P1 : P1_P3 -- release");
	      exit(30);
       }
       
       
     
      /*   Move from Tank A to Tank B  */
      // =================================  distance = memptr-> location[0][0];
      #ifdef __PRINT1__
      printf("heeeeeeeeeer: d:%d, e:%d\n",distance ,endPoint);   
      fflush(stdout);
      #endif
      
      while(distance < endPoint ){
      /* Print t_dis on screen:*/
	      printf("\nP%d1",player_team);
	      fflush(stdout);
	      for(int j=0; j< distance; j++){
		     printf("-");        
	       }
	       printf("\n");
	       fflush(stdout);
	             
	   
	       /* Change the location of player 1 */
	       
	       /* Block P2 from reading */
	       acquire.sem_num = READ;
	       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
		       perror("semop -- P1 : P1_P2  -- acquire");
		       exit(31);
	       }
	       /*Block P3 from reading*/
	       acquire.sem_num = READ;
	       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
		       perror("semop -- P1 : P1_P3  -- acquire");
		       exit(32);
	       }
	       
	       /* Set write to P1 */
	       release.sem_num = WRITE;
	       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
		     perror("semop -- P1 : P1_P2 -- release");
		     exit(33);
	       }
	       release.sem_num = WRITE;
	       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
		     perror("semop -- P1 : P1_P3 -- release");
		     exit(34);
	       }
	       
	  
	       /*Change: */ 
	       memptr-> location[team_index][0] += speed ;
	       distance = memptr-> location[team_index][0];
	       //********
	     
	       /* Access Read To P2 and P3  */
	       acquire.sem_num = WRITE;
	       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
		      perror("semop -- P1 : P1_P2  -- acquire");
		      exit(35);
	       }
	       acquire.sem_num = WRITE;
	       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
		      perror("semop -- P1 : P1_P3  -- acquire");
		      exit(36);
	       }	       
	       /* Set read to P2 & P3 */
	       release.sem_num= READ;
	       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
		      perror("semop -- P1 : P1_P2 -- release");
		      exit(37);
	       }
	       release.sem_num= READ;
	       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
		      perror("semop -- P1 : P1_P3 -- release");
		      exit(38);
	       }
	       	       
	       sleep(3);       
       }
        
       /* P1 reachead the Tank on the other side  
           or
          he fell to the ground and lost the water 
       ============================================== */ 
       #ifdef __PRINT1__   
       YELLOW;
       printf("\nP1 reachead the Tank on the other side or he fell to the ground and lost the water\n\n");
       fflush(stdout);
       WHITE;
       #endif
       
       /* P1 may be fall accidentaly [ randomly ] */
       max= 20 ; min= 0;
       fall_prob = (rand() % (max - min + 1)) + min;
       printf("\n  fall prob: %d \n",fall_prob);
       fflush(stdout);
       switch(fall_prob){
            case  8 ... 20 :
                 fell_on_ground=1;
                 break;
            default:
                 fell_on_ground=0; 
                 break;
       }
       
       /* P1 reached the tank on the other side */
      /* ======================================== */   
       if ( !fell_on_ground ){
       
	       /* In case he reached B */
	       /* The water he lost along the way: */
	       max = (int)(0.30 * BagSize ) ; min = 0 ;
	       lost=(rand() % (max - min + 1)) + min;
	       //#ifdef __PRINT1__
	       printf("The water P1%d  lost along the way: %d \n\n",player_team,lost);
	       fflush(stdout);
	       //#endif
	       

	      /*  Add water in the bag to Tank B :  */
	    
	      /* Block Read on Tank B */
	       acquire.sem_num = READ;
	       if ( semop(semid_to, &acquire, 1) == -1 ) {
		       perror("semop -- P1 -- acquire");
		       exit(39);
	       }
	       /* Block P2 from reading */
	        acquire.sem_num = READ;
	       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
		       perror("semop -- P1 : P1_P2  -- acquire");
		       exit(40);
	       }
	       /*Block P3 from reading*/
	        acquire.sem_num = READ;
	       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
		       perror("semop -- P1 : P1_P3  -- acquire");
		       exit(41);
	       }	       
	       
	       /* Set write to P1 */
	       /* 1. Tank B */
	        release.sem_num = WRITE;
	       if ( semop(semid_to, &release, 1) == -1 ) {
		     perror("semop -- P1 -- release");
		     exit(42);
	       }
	       /* 2. P2 */
	        release.sem_num = WRITE;
	       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
		     perror("semop -- P1 : P1_P2 -- release");
		     exit(43);
	       }
	       /* 3. P3 */
	        release.sem_num = WRITE;
	       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
		     perror("semop -- P1 : P1_P3 -- release");
		     exit(44);
	       }
	       	  
	      /*Change: */   
	      /* check team*/
	      if(player_team == 1){
	             memptr-> water_in_bag_P1 -= lost;
	             memptr-> Amount_Tank_B += memptr-> water_in_bag_P1;
	      }
	      else if(player_team == 2){
	      	     memptr-> water_in_bag_P2 -= lost;
	             memptr-> Amount_Tank_D += memptr-> water_in_bag_P2;
	      }
	       
	       
	       /* Time needed to fill the Tank B */
	       sleep(2);
	      #ifdef __PRINT1__
	       GREEN;     
             if(player_team == 1){
	             printf("> P%d1 filled tank B side with %d\n",player_team, memptr-> water_in_bag_P1);
	      	     fflush(stdout);   
	      	     printf("Amount Tank B : %d\n",memptr-> Amount_Tank_B);  
	      	     fflush(stdout);        
	      }
	      else if(player_team == 2){
	      	     printf("> P%d1 filled tank D side with %d\n",player_team, memptr-> water_in_bag_P2);
	      	     fflush(stdout);
	      	     printf("Amount Tank D : %d\n",memptr-> Amount_Tank_D); 
	      	     fflush(stdout);
	      }
	       WHITE;
	      #endif 
	       
	     //======
	       
	        /* Access reading on Tank B*/
	       acquire.sem_num = WRITE;
	       if ( semop(semid_to, &acquire, 1) == -1 ) {
		      perror("semop -- P1 -- acquire");
		      exit(45);
	       }
	       release.sem_num= READ;
	       if ( semop(semid_to, &release, 1) == -1 ) {
		      perror("semop -- P1 -- release");
		      exit(46);
	       }	       
	       
	       /* Access Read To P2 and P3  */
	       acquire.sem_num = WRITE;
	       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
		      perror("semop -- P1 : P1_P2  -- acquire");
		      exit(47);
	       }
	       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
		      perror("semop -- P1 : P1_P3  -- acquire");
		      exit(48);
	       }
	       
	       /* Set read to P2 & P3 */
	       release.sem_num= READ;
	       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
		      perror("semop -- P1 : P1_P2 -- release");
		      exit(49);
	       }
	       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
		      perror("semop -- P1 : P1_P3 -- release");
		      exit(50);
	       }
   
	            
       }
       
       // In Case he fill to the ground: (he will return to Tank A & set amount of water in bag to 0 )
       /* ========================================================== */   
       else if(fell_on_ground ){
               
               RED;
       	printf("  P1%d fills to the ground.\n\n",player_team);
       	fflush(stdout);
       	WHITE;
             #ifdef __PRINT1__  
             /* check team*/
	      if(player_team == 1){
		     printf("Amount Tank B : %d\n",memptr-> Amount_Tank_B);                  
	      }
	      else if(player_team == 2){
	      	     printf("Amount Tank C : %d\n",memptr-> Amount_Tank_C);
	      	     	     
	      }
              #endif 
               
       	
       	
 
             /* set amount of water in P1 bag to 0 */
               acquire.sem_num = READ;
       	/* Block P2 from reading */
	       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
		       perror("semop -- P1 : P1_P2  -- acquire");
		       exit(51);
	       }
	       acquire.sem_num = READ;
	       /*Block P3 from reading*/
	       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
		       perror("semop -- P1 : P1_P3  -- acquire");
		       exit(52);
	       }
	       
	       /* Set write on P1  */
	       /* 1. P2 */
	       release.sem_num = WRITE;
	       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
		     perror("semop -- P1 : P1_P2 -- release");
		     exit(53);
	       }
	       /* 2. P3 */
	       release.sem_num = WRITE;
	       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
		     perror("semop -- P1 : P1_P3 -- release");
		     exit(54);
	       }
       	
              /* Change*/
       	 
       	
      	    if(player_team == 1){
	            memptr-> water_in_bag_P1 =0;
	      }
	      else if(player_team == 2){
	      	     memptr-> water_in_bag_P2 =0;
	      }
       	
       	//memptr-> location[0][0] = 0 ;
       	//distance= memptr-> location[0][0];
       	
              /* ===== */
       	
       	/* Access Read To P2 and P3  */
	       acquire.sem_num = WRITE;
	       if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
		      perror("semop -- P1 : P1_P2  -- acquire");
		      exit(55);
	       }

       	acquire.sem_num = WRITE;
	       if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
		      perror("semop -- P1 : P1_P3  -- acquire");
		      exit(56);
	       }
	       
	       /* Set read to P2 & P3 */
	       release.sem_num= READ;
	       if ( semop(semid_P1_P2, &release, 1) == -1 ) {
		      perror("semop -- P1 : P1_P2 -- release");
		      exit(57);
	       }
	       release.sem_num= READ;
	       if ( semop(semid_P1_P3, &release, 1) == -1 ) {
		      perror("semop -- P1 : P1_P3 -- release");
		      exit(58);
	       }
       	
       	
       }
       
       
       printf(" \n____________________________ \n\n");
       
       /* Read the amount of water in tank A */
	 acquire.sem_num = READ;
	 if ( semop(semid_from, &acquire, 1) == -1 ) {
		perror("semop -- P1 -- acquire");
	       exit(59);
	  }
	   

	  
	  /* check team*/
	   if(player_team == 1){
		     amount_tank_from = memptr-> Amount_Tank_A ;                   
	   }
	   else if(player_team == 2){
	      	     amount_tank_from  = memptr-> Amount_Tank_C ;   	     	     
	   }
	  
	  release.sem_num = READ;
	  if ( semop(semid_from, &release, 1) == -1 ) {
		perror("semop -- P1 -- acquire");
	       exit(60);
	  }
	  #ifdef __PRINT1__
	  printf("Amount: %d\n",amount_tank_from);
	  fflush(stdout);
	  #endif
	  fell_on_ground =0;
	       

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
          exit(61);
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
