
#include "local.h"

int pid_soldier[MAX_SOLIDER];
int Max=10;

int call=0,roundn=0;
char cmd[100];







char round_num [10][5] = {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th"};



void handle_sigusr2(int sig);
void print();


int main(){

	
	int f_army=0, s_army=0;
	
	srand(time(0));
	
  
	
	/* handel signal SIGUSR1*/
	struct sigaction sa={ 0 };
   	sa.sa_handler= & handle_sigusr2;
   	sigaction(SIGUSR2,&sa,NULL);
	
	/*  CREATE FIFO FILE : (Parent)*/
	if(mkfifo(serverfifo,0777)== -1){
	     if (errno != EEXIST ){
		printf(" Could not craet fifo file \n");
		return(1);
	     }
	}
	
	
	//int stop_war=0;
	/* remove file if exsist */
	sprintf(cmd, "rm -f %s",File);
     	system(cmd);
	
	while(f_army < 5 && s_army < 5){
	Max=MAX_SOLIDER;
	/* at beganing parent process will creat random location for soliders */
	int location_of_soldier[MAX_SOLIDER][2];
	
	/* remove file if exsist */
     	
     	
	/* Generate random location of soldiers on the battlefield */
	for(int i=0;i<2;i++){   
	for(int k=0;k<MAX_SOLIDER;k++){  

	    if (i==0 && k==0 || i==1 && k==0){
	      int randomlocation=rand() %50+ 1;
	      location_of_soldier[k][i]=randomlocation;
	    }
	    else{
		      int check2=0;
			   
		      while (check2!=1){
				int randomlocation=rand() %50 + 1;
				int check=0;
				
				for (int j=0;j<Max;j++){
					if (location_of_soldier[j][i] == randomlocation)
					 check=check+1;
				       
			      }
			      if (check==0){
				  check2=1;
				  location_of_soldier[k][i]=randomlocation;
			       }
			       else{
				int randomlocation=rand() %50+ 1;
			}
		}
	}
	}
	}
	
	#ifdef __PRINT1__
	/* print the x,y location on screen */
	for(int k=0;k< MAX_SOLIDER ;k++){
		for (int i = 0; i < 2; i++) {     
			printf("%d ", location_of_soldier[k][i]);   

		}   
		printf("\n");
	}
	#endif
	
	/* Open file to write locations */
	FILE *out_file = fopen("location.txt", "w+"); // read and write file
	/* test for file not existing */
        if (out_file == NULL){   
		perror("Could not open file\n"); 
	  	exit(-1); // must include stdlib.h 
        } 
	
	/* print the x,y location on file */
	for(int k=0; k< MAX_SOLIDER ;k++){
	   	for (int i = 0; i < 2; i++) {     
		 	fprintf(out_file,"%d", location_of_soldier[k][i]); 
		 	if (i==0){
		 		fprintf(out_file," ");
		 	}    
	   	}   		
	  	fprintf(out_file,"\n");   	
	}
	fclose(out_file);
	
	/* Creat the soldirs */
	for ( int i = 0; i < MAX_SOLIDER; i++ ) {
	    	char string[10]="";
	    	
	    	/* aftar creation we need to pass the location of him */
	    	sprintf(string , "%d %d", location_of_soldier[i][0],location_of_soldier[i][1]);
	    	
		switch ( pid_soldier[i]=fork() ) {
		case -1:
		     perror("Fork failure ");
		     exit(-3);

		case 0: /* In the child */

		     sleep(5);
		     fflush(stdin);
		     execlp("./soldier", "soldier" ,string,(char *) NULL);
		     perror("Exec failure ");
		     return -4;
		break; 

		default:/* In the parent */
		;	

		}
		/* sleep for 1 sec between each creation of child */
		sleep(1);
	}
	
	#ifdef __PEINT2__
	for(int i=0;i< MAX_SOLIDER;i++){
	     printf("%d\n",pid_soldier[i]);
	     fflush(stdout);
	}
	#endif
	
	/* File for PID's and x,y locatin */
	FILE *PID_file = fopen(PID_solider, "w+"); 
	/* test for file not existing */
        if (PID_file == NULL){   
	   	perror("Could not open file\n"); 
	 	exit(-1); 
        } 
         
	int army_soldier= MAX_SOLIDER/2;
	int army; /* 1: 1st army 2: 2nd one*/
	for(int k=0;k< MAX_SOLIDER ;k++){
		if(k<(army_soldier))
			army=1;
		else
			army=2;
		
		fprintf(out_file,"%d %d\n",army, pid_soldier[k]);      	
	}
	fclose(PID_file);
	
	sprintf(cmd, "rm -f num.txt");
	system(cmd);
	
	/* Just for numbering */		
	for(int i=0;i< army_soldier;i++){
	    sprintf(cmd, "echo %d >> num.txt",i+1);
	    system(cmd);
	
	}
	
	/* Merge PID's and location in the same file */
	sprintf(cmd, "paste -d' ' %s location.txt > %s ",PID_solider,File);
	system(cmd);
	/* Remove files */
	sprintf(cmd, "rm -f location.txt | rm -f %s ",PID_solider);
	system(cmd);
	
	
     	
     	print();
     	
	
	
	/* at the end of round */
	
	

	
	while(Max != 0);
	/* remove fifos*/
	
	
	/* */
	f_army=5;
	}
	
	
	
	for (int j=0; j< MAX_SOLIDER; j++){
		sprintf(cmd, "rm -f fifo%d",pid_soldier[j]);
	 	system(cmd);
	
	}
	
	
	
	
	
	
	
	sprintf(cmd, "rm -f location.txt | rm -f %s",PID_solider);
	system(cmd);

}


void handle_sigusr2(int sig){
	WHITE;
	printf(" I Got it my son ! \n");
	fflush(stdout);
	int soldier_pid;
	
	sprintf(cmd, "rm -f fifo%d  ",soldier_pid);
	 system(cmd);
	 
	int fd = open( serverfifo ,O_RDONLY);
	 if(fd == -1 ){
	 printf("h222");
	 fflush(stdout);
	     exit (3);
	 }
	 if(read( fd, &soldier_pid ,sizeof(soldier_pid)) == -1){
	 printf("h222");
	 fflush(stdout);
	 	exit(4);
	 }
	 printf("soldier_pid: %d\n",soldier_pid);
	 fflush(stdout);
	 
	 close(fd);
	 if (kill(soldier_pid , SIGKILL) == -1) {
         perror("kill");
         exit(-5);
         }
         
         sprintf(cmd, "sed -i '/%d/d' %s > g.txt ",soldier_pid,File);
	 system(cmd);
	 sprintf(cmd, "cat g.txt > %s ",File);
	 system(cmd);
	 sprintf(cmd, "rm -f g.txt");
	 system(cmd);

         
	
	 
	 /*UPDATE SOLDIERS ARRAY*/
	 sprintf(cmd, "cat %s | cut -d ' ' -f 2  > team1.txt",File);
	
	
	 Max-=Max;


}




void print(){
	
   	RED;
   	printf("\n  ==============================================\n");
   	printf(" ====          THE WAR HAS BEGUN             ====\n");
   	printf("==================================================\n\n");
	WHITE;
   	printf("                   %s ROUND                 \n\n",round_num[roundn]);
   	YELLOW;
   	printf("       1st ARMY                2nd ARMY    \n");
   	printf("      ==========      VS.     ===========    \n");
   	WHITE;
   	sprintf(cmd, "cat %s | cut -d ' ' -f 2 | head -%d > team1.txt",File,(MAX_SOLIDER/2));
   	system(cmd);
   	sprintf(cmd, "cat %s | cut -d ' ' -f 2 | tail -%d > team2.txt",File,(MAX_SOLIDER/2));
	system(cmd);
	sprintf(cmd, "paste -d'. ' num.txt team1.txt > f1.txt ");
	system(cmd);
	sprintf(cmd, "paste -d'. ' num.txt  team2.txt > f2.txt ");
	system(cmd);
	sprintf(cmd, "rm -f num.txt | rm -f team1.txt | rm -f team2.txt");
	system(cmd);	
   	sprintf(cmd, "sed -i 's/^/\t/' f1.txt ");
   	system(cmd);
   	sprintf(cmd, "sed -i 's/^/\t\t/' f2.txt ");
   	system(cmd);
   	sprintf(cmd, "paste -d'\t  ' f1.txt f2.txt ");
	system(cmd);
	sprintf(cmd, "rm -f f1.txt | rm -f f2.txt");
	system(cmd);
   	printf("\n\nRecharge rifles..\n ");

}
