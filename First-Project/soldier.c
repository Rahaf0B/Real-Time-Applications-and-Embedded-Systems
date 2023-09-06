/*
 * The soldier process
 */

#include "local.h"
#define File "info.txt"

char round_num [10][5] = {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th"};
int Max=10;


typedef struct { 
 double distance[MAX_SOLIDER];
 int pids[MAX_SOLIDER];
 int army[MAX_SOLIDER];/* 1: 1st Army, 2: 2nd one */
} soldiers;

/* functions */
soldiers distance(FILE * file_pointer);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);
int find_index();

/* var */
int x_y[2];
int recharge_time ,call ,hurt_resevrd;
int healthy=100; // At beginning, the soldier is healthy, Uninjured 
int my_army;
soldiers dist;
char privatefifo [20];

/*  START MAIN  */
int main(int argc, char *argv[]){

	void signal_catcher(int);
	srand(time(0));
	
   	struct sigaction sa={ 0 };
   	sa.sa_handler= & handle_sigusr1;
   	sigaction(SIGUSR1,&sa,NULL);
   	
   	struct sigaction sa2={ 0 };
   	sa2.sa_handler= & handle_sigusr2;
   	sigaction(SIGUSR2,&sa2,NULL);
   	
   	
   	
   	/*  CREATE FIFO FILE : */
	
	sprintf(privatefifo, "fifo%d",getpid());
	#ifdef __PRINT__	
	printf("privatefifo:  %s \n",privatefifo);
	fflush(stdout);
	#endif
	
	if(mkfifo(privatefifo,0777)== -1){
	     if (errno != EEXIST ){
		printf(" Could not craet fifo file \n");
		return(1);
	     }
	}
	
	#ifdef __PRINT__	
	printf(" CREATED \n");
	fflush(stdout);
	#endif
	
	

/* wait the parent till creat all soldiers, So that the soldier can know all PID's */
   sleep(7);
   while(healthy > 5){

   FILE * fp;
   
   
   //int location[2];
   
   
        /* Find soldier location */
        char * token1 = strtok(argv[1], " "); 
        int m=0;
        while(  token1!= NULL ) {
   	
   	   x_y[m]= atoi(token1);
   	   token1 = strtok(NULL, " ");
   	   m++;
        }
         
   	#ifdef __PRINT__
	   printf("x_y: %d, %d\n",x_y[0],x_y[1]);
	   fflush(stdout);
	#endif
	
	fp = fopen(File, "r");
	dist = distance(fp);
        fclose(fp); 
	 /* print soldiers info.*/
	 #ifdef __PRINT2__ 
	    for(int i=0;i<Max;i++){
	        printf("\ndistance: %f  army: %d  pid: %d\n",dist.distance[i], dist.army[i],dist.pids[i]);
	        fflush(stdout); 
	    }
	 #endif
	 
	 
	 
	 /* find solider army */
	 int soldier_pid= getpid();
	 for(int i=0;i<Max;i++){
	 	if(dist.pids[i] == soldier_pid ){
	 		my_army= dist.army[i];
	 		#ifdef __PRINT2__
	 		   printf("myarmy: %d\n",my_army);
	 		   fflush(stdout);
	 		#endif
	 	}
	 
	 }
	 
	 /* find minimum distance  */
	 
	 double mindist= 100;
	 int index;
	 for(int i=0;i<Max;i++){
	 if (dist.distance[i] < mindist && dist.distance[i] != 0 && dist.army[i]!= my_army ){
	 mindist=dist.distance[i];
	 index=i;
	 }
	
	 }
	 #ifdef __PRINT__
	     printf("mindis : %f\n",mindist);
	     fflush(stdout);
	     printf("pid of closest solider: %d\n",dist.pids[index]);
	     fflush(stdout);
	 #endif
	 
	 
	 
	 /* Generate Random number [ Time needed to recharge rifles with bullets ] */
	 
	 //====================================
	 
	 #ifdef __recharge__
	int upper=7, lower=2;
	 recharge_time = (rand() % (upper - lower + 1)) + lower;
	 printf("recharge_time :  %d\n",recharge_time);
	 
	 alarm(recharge_time);
	 
	 if ( sigset(SIGALRM, signal_catcher) == SIG_ERR ) {
    		perror("Sigset can not set SIGALRM");
    		exit(SIGINT);
  	}
  	
  	while(1);
  	
	#endif
	
	/* Find the probability to hurt the enemy soldiers */
	
	int upper=100,lower=0;
	int soldier_index= find_index();
	int random_prob = (rand() % (upper - lower + 1)) + lower;
	#ifdef __PRINT__
	     printf("random_prob:  %d\n",random_prob);
	     fflush(stdout);
	#endif
	double probability_to_hurt = ((random_prob/10.0)*(healthy/ ceil(mindist)));
	#ifdef __PRINT__
	     printf("probability_to_hurt:  %f\n",probability_to_hurt);
	     fflush(stdout);
	#endif
	int hurt=0, hit=0;
	int ph= floor(probability_to_hurt);
	YELLOW;
	int ar= my_army-1;
	
	switch(ph){
		case 0 ... 24 :
		   printf("The soldier %d from %s army fired his gun .. No one was hurt\n",getpid(),round_num[ar]);
		   fflush(stdout);
		   hurt=0;
		   break;
		   
		case 25 ... 49 :
		   hit=rand() % 2;
		   if (hit){
		      	printf("The soldier %d from %s army fired his gun .. A blow to the HAND of %d  \n",getpid(),round_num[ar], dist.pids[index]);	
		      	fflush(stdout);
		   }else{
			printf("The soldier %d from %s army fired his gun .. A blow to the LEG of %d \n",getpid(),round_num[ar], dist.pids[index]);
			fflush(stdout);
		   }
		  
		   hurt=1;
		   break;
		   
		case 50 ... 74:
		   hit=rand() % 3;
		   if (hit==1){
			printf("The soldier %d from %s army fired his gun .. A blow to the NECK of %d \n",getpid(), round_num[ar], dist.pids[index]);	
			fflush(stdout);
		   }else if(hit == 2){
			printf("The soldier %d from %s army fired his gun .. A blow to the CHEST of %d \n",getpid(),round_num[ar], dist.pids[index]);
			fflush(stdout);
		   }else{
			printf("The soldier %d from %s army fired his gun .. A blow to the ABDOMEN of %d \n",getpid(),round_num[ar], dist.pids[index]);
			fflush(stdout);
		   }
		   
		   hurt=2;
		   break;
		default:
		   printf("The soldier %d from %s army fired his gun .. A blow to the HEAD of %d \n",getpid(),round_num[ar], dist.pids[index]);
		   fflush(stdout);
		   hurt=3;	
	}
	
	WHITE;
	
	int pid_sendto = dist.pids[index];
	
	
	if(hurt != 0){
	/* Send message to the nearby solider that he was shot
	 [ the value of hurt ]
	   USING PRIVATE FIFO  
	 */ 
	 
	 char enemy_soldier [20];
	 sprintf(enemy_soldier, "fifo%d",dist.pids[index]);
	 #ifdef __PRINT__	
	 printf("enemy_soldier:  %s \n",enemy_soldier);
	 fflush(stdout);
	 #endif
	 
	 /* befor open the fifo i need to send signal to the 
	 enemy_soldier to open the fifo 
	 */
	 
	 kill(pid_sendto,SIGUSR1);
	 #ifdef __PRINT__
	 printf("SENT MY PID %d TO PID %d \n",getpid(),dist.pids[index]);
	 fflush(stdout);
	 #endif
	 
	 int fd = open(enemy_soldier,O_WRONLY);
	 if(fd == -1 ){
	     return(2);
	 }
	 if(write( fd,&hurt ,sizeof(hurt)) == -1){
	     return(3);
	 }
	 close(fd);
	}
	    
	 /* In case healty less than 5 the soldier will 
	 send msg to parent that he was die */
	 
	 GREEN;
	
	sleep(2);
	   
	 
	}
	
		int pid_min=getpid();
	
	    printf("\n       Soldier %d Died \n",getpid());
	    fflush(stdout);
	    kill(getppid(),SIGUSR2);
	    
	    int fd2 = open( serverfifo,O_WRONLY);
	    if(fd2 == -1 ){
	         return(4);
	     }
	     if(write( fd2,&pid_min ,sizeof(pid_min)) == -1){
	          return(5);
	      }
	      close(fd2);
	     
   
	 
	printf("Max: %d",Max); 
return 0;
	 
}

void signal_catcher(int the_sig){

  printf(" recharge_time for process PID   %d\n",getpid() );
  alarm(recharge_time);  
}


soldiers distance(FILE *file_pointer){

	soldiers soldier;
	double dis;
	int read[4];

	if (file_pointer == NULL) {
	     printf("Unable to open the file!! ");
	     fflush(stdout);
	     exit(1);
	 }
	 
	 char line[15];
	 int j=0;
	 while(fgets(line,sizeof(line),file_pointer)){
	   
	     /* splitting string py space */
	   
	     int len = strlen(line); // get length of line
	     if( line[len-1] == '\n' ) line[len-1] = 0; // remove newline from a string
	     
	     char * token = strtok(line, " ");
	     int k=0;
	     
	     while( token != NULL ) {
	     
	         read[k]= atoi(token);
	         token = strtok(NULL, " ");
	         k++;
	     }
	     
	     soldier.army[j]=read[0];
	     soldier.pids[j]=read[1];
	     
	     #ifdef __PRINT2__ 
	     printf("x: %d, y: %d.  army:%d  pid: %d  ",read[2],read[3],read[0],read[1]);
	     fflush(stdout);
	     #endif
	     int xt=x_y[0]-read[2];
	     int yt=x_y[1]-read[3];
	     
	     dis=sqrt(pow(xt,2)+pow(yt,2)); // gcc soldier.c -o soldier -lm
	     soldier.distance[j]=dis; 
	     j++; 
	     
        }
    
        return soldier;
}


int find_index(){
	 int index;
	 int soldier_pid= getpid();
	 for(int i=0;i<Max;i++){
	 	if(dist.pids[i] == soldier_pid ){
	 		index= i;
	 		
	 	} 
	 }

return index;
}

void handle_sigusr1(int sig){
	
	
	int fd = open(privatefifo ,O_RDONLY);
	 if(fd == -1 ){
	     exit (3);
	 }
	 if(read( fd, &hurt_resevrd ,sizeof(hurt_resevrd)) == -1){
	 	exit(4);
	 }
	 
	 close(fd);
	 //#ifdef __PRINT__
	 printf("hurt_reserved : %d PID %d\n",hurt_resevrd,getpid());
	 fflush(stdout);
	 //#endif
	 
	 	 /* check for injurie */
	 
	 /* in case 0 it dose not do anything */
	    switch(hurt_resevrd){
	       case 1:
	          healthy-= 20;
	          break;
	       case 2:
	          healthy-= 33;
	          break;
	       case 3:
	          healthy-= 50;
	    
	    }
}

void handle_sigusr2(int sig){
         printf(" DOOOOOOOOOOOOOOOOOON! \n");
	 fflush(stdout);
	Max-=1;
	
}








