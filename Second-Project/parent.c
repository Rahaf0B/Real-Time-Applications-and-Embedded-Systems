#include "local.h"


int main(){
  
  // P11, P12, P13, P14
  // P21, P22, P23, P24
  
  static struct  MEMORY memory;
  static ushort  start_val[2] = {1, 0}; /* 1 player can write */
  char  *shmptr;
  struct MEMORY *memptr;
  
  union semun arg; //argA, argB, argC, argD ;
  int semidA, semidB, semidC, semidD , shmid;
  int semid_P1_P2, semid_P1_P3, semid_P2,semid_P3, semid_P4, 
      semid_P21_P2, semid_P21_P3, semid_P22,semid_P23, semid_P24;
  
  
  
  pid_t  players_pid[2][4], pid = getpid();
  char player_team[2], player_num[2];
  
  
   /* Create, attach and initialize the memory segment */
   if ( (shmid = shmget((int) pid, sizeof(memory), IPC_CREAT | 0600)) != -1 ) {
    
          if ( (shmptr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
                perror("shmptr -- parent -- attach");
                exit(1);
          }
          
          memptr = (struct MEMORY *) shmptr;
   }
   else {
         perror("shmid -- parent -- creation");
         exit(2);
   }
  
   /*
   * Create and initialize the semaphores
   */
   
   arg.array = start_val;
   
   /* Tank A : */
    if ( (semidA = semget(K_tankA, 2, IPC_CREAT | 0666)) != -1 ) {
          
           
           if ( semctl(semidA, 0, SETALL, arg) == -1 ) {
                 perror("semctl [A] -- parent -- initialization");
                 exit(3);
           }
    }
    else {
         perror("semget [A] -- parent -- creation");
         exit(4);
    }
    
    /* Tank B: */
    if ( (semidB = semget(K_tankB, 2, IPC_CREAT | 0666)) != -1 ) {
      
           
           if ( semctl(semidB, 0, SETALL, arg) == -1 ) {
                 perror("semctl [B] -- parent -- initialization");
                 exit(5);
           }
    }
    else {
         perror("semget [B] -- parent -- creation");
         exit(6);
    }
    
    
    /* Tank C: */
    if ( (semidC = semget(K_tankC, 2, IPC_CREAT | 0666)) != -1 ) {
   
           
           if ( semctl(semidC, 0, SETALL, arg) == -1 ) {
                 perror("semctl [C] -- parent -- initialization");
                 exit(7);
           }
    }
    else {
         perror("semget [C] -- parent -- creation");
         exit(8);
    }
    
    /* Tank D: */
    if ( (semidD = semget(K_tankD, 2, IPC_CREAT | 0666)) != -1 ) {

           
           if ( semctl(semidD, 0, SETALL, arg) == -1 ) {
                 perror("semctl [D] -- parent -- initialization");
                 exit(9);
           }
    }
    else {
         perror("semget [D]  -- parent -- creation");
         exit(10);
    }
    
    
    
    /******************
       location of P1 : 
    ********************/
    /* With P2*/
    
    if ( (semid_P1_P2 = semget(K_loc_P1_P2, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P1_P2, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P1_P2] -- parent -- initialization");
                 exit(11);
           }
    }
    else {
         perror("semget [P1_P2]   -- parent -- creation");
         exit(12);
    }
   
    
    /* With P3*/
    if ( (semid_P1_P3 = semget(K_loc_P1_P3, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P1_P3, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P1_P3]  -- parent -- initialization");
                 exit(13);
           }
    }
    else {
         perror("semget [P1_P3]   -- parent -- creation");
         exit(14);
    }
    
    /******************
       location of P2 : 
    ********************/
    /* With P4*/
    if ( (semid_P2 = semget(K_loc_P2, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P2, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P2] -- parent -- initialization");
                 exit(15);
           }
    }
    else {
         perror("semget [P2]   -- parent -- creation");
         exit(16);
    }
    
    
    /******************
       location of P3 : 
    ********************/
    /* With P4*/
    if ( (semid_P3 = semget(K_loc_P3, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P3, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P3] -- parent -- initialization");
                 exit(17);
           }
    }
    else {
         perror("semget [P3]   -- parent -- creation");
         exit(18);
    }
  
     /******************
       location of P4 : 
    ********************/
    /* With P2 Or P3 */
    if ( (semid_P4 = semget(K_loc_P4, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P4, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P4] -- parent -- initialization");
                 exit(19);
           }
    }
    else {
         perror("semget [P4]   -- parent -- creation");
         exit(20);
    } 
    
    
    //================== TEAM 2 ===============
    
     /******************
       location of P21 : 
    ********************/
    /* With P2*/
    if ( (semid_P21_P2 = semget(K_loc_P21_P2, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P21_P2, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P21_P2] -- parent -- initialization");
                 exit(11);
           }
    }
    else {
         perror("semget [P21_P2]   -- parent -- creation");
         exit(12);
    }
    
    /* With P3*/
    if ( (semid_P21_P3 = semget(K_loc_P21_P3, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P21_P3, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P1_P3]  -- parent -- initialization");
                 exit(13);
           }
    }
    else {
         perror("semget [P1_P3]   -- parent -- creation");
         exit(14);
    }
    
    /******************
       location of P2 : 
    ********************/
    /* With P4*/
    if ( (semid_P22 = semget(K_loc_P22, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P22, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P22] -- parent -- initialization");
                 exit(15);
           }
    }
    else {
         perror("semget [P22]   -- parent -- creation");
         exit(16);
    }
    
    
    /******************
       location of P3 : 
    ********************/
    /* With P4*/
    if ( (semid_P23 = semget(K_loc_P23, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P23, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P23] -- parent -- initialization");
                 exit(17);
           }
    }
    else {
         perror("semget [P23]   -- parent -- creation");
         exit(18);
    }
  
     /******************
       location of P4 : 
    ********************/
    /* With P2 Or P3 */
    if ( (semid_P24 = semget(K_loc_P24, 2, IPC_CREAT | 0666)) != -1 ) {
           
           
           if ( semctl(semid_P24, 0, SETALL, arg) == -1 ) {
                 perror("semctl [P24] -- parent -- initialization");
                 exit(19);
           }
    }
    else {
         perror("semget [P24]   -- parent -- creation");
         exit(20);
    } 
    
    
    
    
    
  
  
   /* Prepare  The Game: */
   /* At the beganing of the raund Tank A, And Tank C are Full */
   
   
   /* Set the amount of water in tank A */
   
   acquire.sem_num = WRITE;
   if ( semop(semidA, &acquire, 1) == -1 ) {
         perror("semop -- Parent Tank A -- acquire");
         exit(21);
    }
    memptr-> Amount_Tank_A = TSizeA ;
    
    release.sem_num = READ;
    
    if ( semop(semidA, &release, 1) == -1 ) {
      perror("semop -- Parent Tank A -- release");
      exit(22);
    }
   
    YELLOW;
    printf("\n> P: Tank A is filled with %d Liters.\n\n", TSizeA );
    fflush(stdout);
    WHITE;
    
    
   /* Set the amount of water in tank B */
   
   acquire.sem_num = WRITE;
   if ( semop(semidB, &acquire, 1) == -1 ) {
         perror("semop -- Parent Tank B -- acquire");
         exit(23);
    }
    memptr-> Amount_Tank_B = 0 ;
    memptr-> stop = 0;
    
    release.sem_num = READ;
    
    if ( semop(semidB, &release, 1) == -1 ) {
      perror("semop -- Parent Tank B -- release");
      exit(24);
    }
   
    YELLOW;
    printf("\n> P: Tank B Is Empty.\n\n");
    fflush(stdout);
    WHITE;
    
    
    /* Set the amount of water in tank C */
   
   acquire.sem_num = WRITE;
   if ( semop(semidC, &acquire, 1) == -1 ) {
         perror("semop -- Parent Tank C -- acquire");
         exit(25);
    }
    memptr-> Amount_Tank_C = TSizeA ;
    
    release.sem_num = READ;
    
    if ( semop(semidC, &release, 1) == -1 ) {
      perror("semop -- Parent Tank C -- release");
      exit(26);
    }
   
    YELLOW;
    printf("\n> P: Tank C is filled with %d Liters.\n\n", TSizeA );
    fflush(stdout);
    WHITE;
    
    
   /* Set the amount of water in tank D */
   
   acquire.sem_num = WRITE;
   if ( semop(semidD, &acquire, 1) == -1 ) {
         perror("semop -- Parent Tank D -- acquire");
         exit(27);
    }
    memptr-> Amount_Tank_D = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semidD, &release, 1) == -1 ) {
      perror("semop -- Parent Tank D -- release");
      exit(28);
    }
   
    YELLOW;
    printf("\n> P: Tank D Is Empty.\n\n");
    fflush(stdout);
    WHITE;
    
    
  //========================================  
    
    
   /* Set P11 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P1_P2, &acquire, 1) == -1 ) {
         perror("semop -- Parent P1_P2 -- acquire");
         exit(29);
    }
    
   if ( semop(semid_P1_P3, &acquire, 1) == -1 ) {
         perror("semop -- Parent P1_P3 -- acquire");
         exit(30);
    }
    
    memptr-> location[0][0] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P1_P2, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P2 -- release");
      exit(31);
    }
    
    if ( semop(semid_P1_P3, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P3 -- release");
      exit(32);
    }
    
    
    /* Set P12 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P2, &acquire, 1) == -1 ) {
         perror("semop -- Parent P2 -- acquire");
         exit(33);
    }

    memptr-> location[0][1] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P2, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P2 -- release");
      exit(34);
    }
    
    
   /* Set P13 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P3, &acquire, 1) == -1 ) {
         perror("semop -- Parent P3 -- acquire");
         exit(35);
    }

    memptr-> location[0][2] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P3, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P3 -- release");
      exit(36);
    }
  
    
    /* Set P14 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P4, &acquire, 1) == -1 ) {
         perror("semop -- Parent P4 -- acquire");
         exit(37);
    }

    memptr-> location[0][3] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P4, &release, 1) == -1 ) {
      perror("semop -- Parent P4 -- release");
      exit(38);
    }
    
    
 //=============================   
    
       /* Set P11 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P21_P2, &acquire, 1) == -1 ) {
         perror("semop -- Parent P1_P2 -- acquire");
         exit(29);
    }
    
   if ( semop(semid_P21_P3, &acquire, 1) == -1 ) {
         perror("semop -- Parent P1_P3 -- acquire");
         exit(30);
    }
    
    memptr-> location[1][0] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P21_P2, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P2 -- release");
      exit(31);
    }
    
    if ( semop(semid_P21_P3, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P3 -- release");
      exit(32);
    }
    
    
    
    /* Set P22 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P22, &acquire, 1) == -1 ) {
         perror("semop -- Parent P22 -- acquire");
         exit(33);
    }

    memptr-> location[1][1] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P22, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P2 -- release");
      exit(34);
    }
    
   /* Set P23 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P23, &acquire, 1) == -1 ) {
         perror("semop -- Parent P33 -- acquire");
         exit(35);
    }

    memptr-> location[1][2] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P23, &release, 1) == -1 ) {
      perror("semop -- Parent P1_P3 -- release");
      exit(36);
    }
    
    
    /* Set P24 to location 0 */

   acquire.sem_num = WRITE;
   if ( semop(semid_P24, &acquire, 1) == -1 ) {
         perror("semop -- Parent P4 -- acquire");
         exit(37);
    }

    memptr-> location[1][3] = 0 ;
    
    release.sem_num = READ;
    
    if ( semop(semid_P24, &release, 1) == -1 ) {
      perror("semop -- Parent P4 -- release");
      exit(38);
    }

   
   int size_of_cup=(int)(BagSize/5);
   
   printf("Size of cup %d\n",size_of_cup);
    fflush(stdout);
    
  
  /* ===================
   * =  FORK PLAYERS:  = 
   * =================== 
   */

  for( int i=0 ; i < 2 ; i++ ){
  
  
          sprintf(player_team,"%d",(i+1));
  
  	  // Create water transfer player P11 --> p[0][0], P21 --> p[1][0]
	  switch( players_pid[i][0] = fork() ){
	  	case -1: // failure
	  		perror("Fork Player 1 Failure");
	  		return -1;
	  	case 0: // In the child P1 
	  	         
	  		execlp("./p1", "player1",player_team,(char *) NULL);
			perror("Exec failure ");
	  		return -2;
	  	//default: // In the parent
	  		
	  		//printf("My process parent ID is %d\n", getpid());  
	  }
	  
	  // Create annoying players P12, P13,  P22, P23
	  for ( int j=1 ; j< 3 ; j++){
		  // Create P12 --> p[0][1], P22 --> p[1][1]
		  // P13 --> p[0][2], P23 --> p[1][2]
		  switch( players_pid[i][j] = fork() ){
		  	case -1: // failure
		  		perror("Fork P2,P3 Failure");
		  		return -3;
		  	case 0: // In the child P2, P3 
		  	        sprintf(player_num,"%d",(j+1));
		  		execlp("./annoying", "annoying_players" ,player_team,
		  		player_num, (char *) NULL);
				perror("Exec failure ");
		  		return -4;
		  	//default: // In the parent
		  
		  		//printf("My process parent ID is %d\n", getpid());  
		  }
	  }
	  // Create the annoying sand players P14 --> p[0][3], P24 --> p[1][3]
	  switch( players_pid[i][3] = fork() ){
	  	case -1: // failure
	  		perror("Fork Player 4 Failure");
	  		return -1;
	  	case 0: // In the child P1 
	  	        
	  		execlp("./sand", "sand_player" ,player_team,(char *) NULL);
			perror("Exec failure ");
	  		return -2;
	  	//default: // In the parent
	  		
	  		//printf("My process parent ID is %d\n", getpid());  
	  }
	   
   }
   /*
   int r_time=time_round;
   int stop=memptr-> stop;
   while(!stop && r_time > 0 ){
      r_time -=1;
      stop=memptr-> stop;
      sleep(1);
   }
   memptr-> stop = 0;
   */
   
   
   

   
  
   
      
   
   
   printf("My process parent ID is %d\n", getpid());
   

         
  while ( wait(NULL) > 0);
  
  
  
       if ( semctl(semidA, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semidA");
      exit(6);
    }
    if ( semctl(semidB, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semidB");
      exit(6);
    }
    if ( semctl(semidC, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semidC");
      exit(6);
    }
    if ( semctl(semidD, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semidD");
      exit(6);
    }
    /************/
    if ( semctl(semid_P1_P2, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P1_P2");
      exit(6);
    }
    if ( semctl(semid_P1_P3, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P1_P3");
      exit(6);
    }
    if ( semctl(semid_P2, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P2");
      exit(6);
    }
    if ( semctl(semid_P3, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P3");
      exit(6);
    }
    
    
    if ( semctl(semid_P4, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P4");
      exit(6);
    }
    if ( semctl(semid_P21_P2, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P21_P2");
      exit(6);
    }
    if ( semctl(semid_P21_P3, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P21_P3 ");
      exit(6);
    }
    if ( semctl(semid_P22, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove semid_P22");
      exit(6);
    }
    
    if ( semctl(semid_P23, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove -- semid_P23 ");
      exit(6);
    }
    if ( semctl(semid_P24, 0, IPC_RMID, 0) == -1 ) {
      perror("semctl --remove -- semid_P24");
      exit(6);
    }
  return(0);
  
  
}





