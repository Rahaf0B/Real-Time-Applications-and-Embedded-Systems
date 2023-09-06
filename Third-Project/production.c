#include "local.h"

int produce;
int work=0;
int emp_speed[NUMBER_OF_TECH_EMPLOYEE ];

pthread_mutex_t    S1_mutex,
				   S2_mutex,
                   Truck_A_mutex ,
                   Truck_B_mutex ,
				   Truck_product_mutex,
				   Manufacturing_Line_conter_Mutex,
				   Manufacturing_Line_conter_boxes_Mutex,
				   Manufacturing_Line_product_Mutex,
				   production_line_mutex[NUMBER_OF_PRODUCTION_LINE];
				   

pthread_cond_t  fill_S1_A,
				L3_cond,
				detergent_cond,
				emplyee_cond[NUMBER_OF_PRODUCTION_LINE][NUMBER_OF_TECH_EMPLOYEE];


pthread_t  first_technical[NUMBER_OF_PRODUCTION_LINE],
		   last_technical[NUMBER_OF_PRODUCTION_LINE];


/* define func. */
void setup();
void start_production ();
void read_file(char *file);
void move_materials_A_B_to_storage_area_S1 (void* id);
void move_materials_manufacturing_lines(void* id);
void production_line_process (void* id);
void first_technical_employee (void* arg);
void employee_steps(void* arg);
void last_technical_employee (void* arg);
void move_boxes_to_trucks(void* arg);
void print_finaly();
void detergent_factory_employee (void* id);

/* define var: */
static int  quantity_of_A_intial, /*The quantity of A in storage area S1*/
            quantity_of_B_intial, /*The quantity of B in storage area S1*/
			number_of_A_trucks,
			capacity_of_A_truck,
			number_of_B_trucks,
			capacity_of_B_truck,
			storage_S1_capacity,
			number_of_product,
			Time_to_end,
			numb_of_L1,
			L1_carried_quantity,
			numb_of_L2,
			numb_of_L3,
			numb_of_detergent,
			number_of_box_to_manufacture,
			Trip_Time,
			number_of_minutes,
			Truck_product_size;


static int truck_A_arrive_S1=0 ,/* 1: arrive S1, 0: if not*/
           truck_B_arrive_S1=0 ; /* 1: arrive S1, 0: if not*/
           
           


static struct Storage_S1 storage_S1;
static struct Storage_S2 storage_S2;
static struct Truck_A trucks_A;
static struct Truck_B trucks_B;
static struct Manufacturing_Lines manufacturing_Line;

static struct Truck_Product Truck_product;


/*============ START MAIN =====================*/
int main(int argc , char *argv[]){

    produce=1;
    
    
    /* The data file must be entered as an argument */
    if(argc != 2){
        fprintf(stderr, "Error: No file Chosen");
        return 1;
    }
    
    /* To generate random numb. */
    srand(time(0));
    /* read user data from a file */
    read_file(argv[1]);
    setup();
    
	
    
    /* *** Mutex Initialization *** */
    pthread_mutex_init( &S1_mutex, NULL);
	pthread_mutex_init( &S2_mutex, NULL);
	pthread_mutex_init( &Truck_A_mutex, NULL);
	pthread_mutex_init( &Truck_B_mutex, NULL);
	pthread_mutex_init( &Truck_product_mutex, NULL);
	pthread_mutex_init( &Manufacturing_Line_conter_Mutex, NULL);
	pthread_mutex_init( &Manufacturing_Line_conter_boxes_Mutex, NULL);
	pthread_mutex_init( &Manufacturing_Line_product_Mutex, NULL);


	for (int i=0; i< NUMBER_OF_PRODUCTION_LINE; i++){
		pthread_mutex_init( &production_line_mutex[i], NULL);
	}
    
    /* *** cond Initialization *** */
    pthread_cond_init( &fill_S1_A, NULL);
	pthread_cond_init( &L3_cond, NULL);
	pthread_cond_init( &detergent_cond, NULL);
	

	


	for (int i=0; i< NUMBER_OF_PRODUCTION_LINE; i++){
		for(int j=0; j< NUMBER_OF_TECH_EMPLOYEE; j++){
			pthread_cond_init(&emplyee_cond[i][j], NULL);
		}

	}
    
    
    
    /* *** Define Threads *** */
    pthread_t L1_employee [numb_of_L1], 
              L2_employee [numb_of_L2],
			  L3_employee [numb_of_L3],
			  detergent_employee[numb_of_detergent],
              production_line [NUMBER_OF_PRODUCTION_LINE];


    int L1[numb_of_L1];
     /* generate threads */
    for(int i=0; i< numb_of_L1; i++ ){
        L1[i]=i;
        if(pthread_create(&L1_employee [i], NULL, (void *)
         move_materials_A_B_to_storage_area_S1, (void*)&L1[i]) != 0){
               perror("Error: Create pthread L1 employee\n ");
               return 2;
        }    
    }

	int L2[numb_of_L2];
     /* generate threads */
    for(int i=0; i< numb_of_L2; i++ ){
        L2[i]=i;
        if(pthread_create(&L2_employee [i], NULL, (void *)
         move_materials_manufacturing_lines, (void*)&L2[i]) != 0){
               perror("Error: Create pthread L2 employee\n ");
               return 3;
        }    
    }

	int L3[numb_of_L3];
     /* generate threads */
    for(int i=0; i< numb_of_L3; i++ ){
        L3[i]=i;
        if(pthread_create(&L3_employee [i], NULL, (void *)
         move_boxes_to_trucks, (void*)&L3[i]) != 0){
               perror("Error: Create pthread L3 employee\n ");
               return 4;
        }    
    }


	int detergent[numb_of_detergent];
     /* generate threads */
    for(int i=0; i< numb_of_detergent; i++ ){
        detergent[i]=i;
        if(pthread_create(&detergent_employee [i], NULL, (void *)
         detergent_factory_employee, (void*)&detergent[i]) != 0){
               perror("Error: Create pthread L2 employee\n ");
               return 3;
        }    
    }
	
	/* START PRODUCTION */
    start_production ();

	int lineproduction[NUMBER_OF_PRODUCTION_LINE];
     /* generate threads */
    for(int i=0; i< NUMBER_OF_PRODUCTION_LINE; i++ ){

        lineproduction[i]=i;
        if(pthread_create(&production_line [i], NULL, (void *)
          production_line_process, (void*)&lineproduction[i]) != 0){
               perror("Error: Create pthread Production Line \n ");
               return 5;
        }    
    }
    
  
    
    
    
    
    int sleep_in_sec= number_of_minutes* 60;
    sleep(sleep_in_sec);  /* TO EDIT (user defined time )*/
    produce=0;

	//printf("               --- End Producing ---                             \n");

	/* wake up all threadswating this cond */
    pthread_cond_broadcast(&fill_S1_A);
	pthread_cond_broadcast(&L3_cond);
	pthread_cond_broadcast(&detergent_cond);
	
    
    
    
    /* wait threads until finish there work*/
    for(int i=0; i< numb_of_L1; i++ ){
        if (pthread_join( L1_employee [i], NULL)!= 0){
            perror("Error: Pthread join L1 employee ");
            return 6;
        }
    }

	/* wait threads until finish there work*/
    for(int i=0; i< numb_of_L2; i++ ){
        if (pthread_join( L2_employee [i], NULL)!= 0){
            perror("Error: Pthread join L2 employee ");
            return 6;
        }
    }
	
	/* wait threads until finish there work*/
    for(int i=0; i< numb_of_L3; i++ ){
        if (pthread_join( L3_employee [i], NULL)!= 0){
            perror("Error: Pthread join L3 employee ");
            return 6;
        }
    }
    
    
    
    /* ***  Mutex Destroy *** */
    pthread_mutex_destroy( &S1_mutex );
	pthread_mutex_destroy( &S2_mutex );
	pthread_mutex_destroy( &Truck_A_mutex );
	pthread_mutex_destroy( &Truck_B_mutex);
	pthread_mutex_destroy( &Truck_product_mutex);
	pthread_mutex_destroy( &Manufacturing_Line_conter_Mutex );
	pthread_mutex_destroy( &Manufacturing_Line_conter_boxes_Mutex);
	pthread_mutex_destroy( &Manufacturing_Line_product_Mutex);
    

    
    printf("\n\n%s                  >>>> The END ... <<<<                           \n\n",BLUE  );
    fflush(stdout);


    return 0;

}



/* read file of input data 
=============================*/
void read_file(char *file){
   
    int data_from_file [NUMB_DATA_USER_DEFINED];
    int data = 0;
    int index=0;
    
    FILE *fptr = fopen (file, "r");
    if(fptr == NULL){
        perror("Error: Open the file");
        exit(1);
    }
    
    while (!feof (fptr)){    
         
        fscanf (fptr, "%d\n", &data);      
        data_from_file[index]= data;
        #ifdef __PRINT1__
        printf ("Data[%d]: %d \n",index, data);
        fflush(stdout);
        #endif
        index= index+1;
    }
    
    if(fclose(fptr) == EOF){
        perror("Error: Close the file");
        exit(2);
    }
    
    quantity_of_A_intial= data_from_file[0];
    quantity_of_B_intial= data_from_file[1];
    number_of_A_trucks=   data_from_file[2];
    capacity_of_A_truck=  data_from_file[3];
    number_of_B_trucks=   data_from_file[4];
    capacity_of_B_truck=  data_from_file[5];
    storage_S1_capacity=  data_from_file[6];
    number_of_product  =  data_from_file[7];
    Time_to_end =         data_from_file[8];
    numb_of_L1 =          data_from_file[9];
    L1_carried_quantity=  data_from_file[10];
    numb_of_L2 =          data_from_file[11];
	numb_of_L3 =          data_from_file[12];
	numb_of_detergent=    data_from_file[13];
	number_of_box_to_manufacture= data_from_file[14],
	Trip_Time           = data_from_file[15];
	number_of_minutes   = data_from_file[16];
	Truck_product_size  = data_from_file[17];
    
}


/* SETUP 
==============*/
void setup(){

	/* Quantity of A & B in S1 */
	storage_S1.quantity_of_A_product_in_S =  quantity_of_A_intial ;
	storage_S1.quantity_of_B_product_in_S =  quantity_of_B_intial ;
	storage_S1.move_A=1;
	storage_S1.move_B=1;
	storage_S2.number_of_boxes=0;

	manufacturing_Line.conter_lines_A=0;
	manufacturing_Line.conter_last_step=0;

	Truck_product.counter=0;

	for(int j=0; j< NUMBER_OF_PRODUCT_TRUCKS ; j++){
		Truck_product.truck_avalabile[j]=1;
	}

	for(int i=0; i< NUMBER_OF_PRODUCTION_LINE; i++){
		manufacturing_Line.manufac_product[i]=0;
	}

	
	
	for (int i=0; i< NUMBER_OF_PRODUCTION_LINE; i++){
		manufacturing_Line.A_in_line[i]=0;
	}

	for (int i=0; i< NUMBER_OF_TECH_EMPLOYEE ; i++){
		emp_speed[i]=(rand() % (Max_speed - Min_speed + 1)) + Min_speed; 
		#ifdef __PRINT2__
		printf(" The speed of employee [%d] is %d\n",(i+1),emp_speed[i]);
		fflush(stdout);
		#endif
	}

	printf("\nNumber of trucks for A :  %d, with capacity: %d                     \n",number_of_A_trucks ,capacity_of_A_truck);
	fflush(stdout);
	printf("Number of trucks for B :  %d, with capacity: %d                       \n",number_of_B_trucks, capacity_of_B_truck );
	fflush(stdout);  
}

// ============ START PRODUCTION ===================
void start_production (){
	printf("\n%sStorage_S1_capacity:  %d                                      \n",WHITE,storage_S1_capacity );
    fflush(stdout);
    pthread_cond_broadcast(&fill_S1_A);  
    printf("\n%s START PRODUCTION ...  %s                                      \n\n",YELLOW,WHITE);
    fflush(stdout);
   
	
}


/* The L1 employees move the materials (either A or B) to the detergent factory and place them in a storage area S1.
=====================================================================================================================*/
void move_materials_A_B_to_storage_area_S1 (void* id){ 
    int my_id = *((int*)id);
    int p1=1, p2=1;
    int L1_carried=0;
    while(produce){	
    
    	/* L1 will waits for a signal from his boss to start working*/
        if(!work){
			#ifdef __PRINT1__
             printf("[%d]: waiting signal ...\n",my_id);
             fflush(stdout);
			 #endif
             pthread_mutex_lock(&S1_mutex);
             pthread_cond_wait(&fill_S1_A, &S1_mutex  );
             work=1;
             pthread_mutex_unlock(&S1_mutex);
             
        }
     	/*L1 will start working until he finishes moving materials.*/
		else{
               /* L1 with id 0: will check how many trucks are needed from A and B   &   order the materials.   */
			if(my_id == 0 && !truck_A_arrive_S1 && !truck_B_arrive_S1){
             	
				pthread_mutex_lock (&S1_mutex );
				storage_S1.number_needed_from_A=  (int)(floor(((2/3.0) * storage_S1_capacity) - storage_S1.quantity_of_A_product_in_S));
				storage_S1.number_needed_from_B=  (int)(floor(((1/3.0) * storage_S1_capacity) - storage_S1.quantity_of_B_product_in_S));

				storage_S1.number_of_truck_nedded_A = (int)(floor(storage_S1.number_needed_from_A / capacity_of_A_truck));
				storage_S1.number_of_truck_nedded_B = (int)(floor(storage_S1.number_needed_from_B / capacity_of_B_truck));

				#ifdef __PRINT1__
				printf("Number_needed_from_A: %d \n",storage_S1.number_needed_from_A );
				fflush(stdout);
				printf("Number_needed_from_B: %d \n\n",storage_S1.number_needed_from_B );
				fflush(stdout);
				#endif

				pthread_mutex_unlock (&S1_mutex );

				/* quantity needed from A must be equal or biger than truck size (untill the truck is fully loaded) */
			
				/* In case needed to import new matarial  from  A  
				=====================================================*/
				if(!truck_B_arrive_S1){
					
					if(storage_S1.number_of_truck_nedded_A > 0 && !truck_A_arrive_S1 ){
						storage_S1.move_A=1;work=1;
						#ifdef __PRINT1__
						printf("number_of_truck_nedded_A: %d \n\n",storage_S1.number_of_truck_nedded_A );
						fflush(stdout);
						#endif


						/* numb. of trucks that will leave factory A (loaded with matarial A )*/
						(storage_S1.number_of_truck_nedded_A >= number_of_A_trucks) ? (trucks_A.trucks_departing = number_of_A_trucks) :  
															(trucks_A.trucks_departing = storage_S1.number_of_truck_nedded_A);
						/* Print just for the first time */
						if(p1){
							printf("%s  %d  Trucks leave Factory A  ", RED, trucks_A.trucks_departing );
							fflush(stdout);
							/* Time needed to arrive S1 */
							for(int j=0; j< 6; j++){
								for(int k=0; k< 100000000; k++);
								printf(". ");
								fflush(stdout);
							}
							printf("\r%s  >> %d Trucks loaded with A arrived  Detergent factory <<                   \n  " ,CYAN, trucks_A.trucks_departing );
							fflush(stdout);
							printf("%s\n" ,WHITE);
							fflush(stdout);
							
						}

						truck_A_arrive_S1=1;

						/* calculate numb. of materials A arrived */
						trucks_A.quantity_arrived = trucks_A.trucks_departing *  capacity_of_A_truck;
						printf("\n\n >> quantity_of_A_arrived : %d                                                 \n" , trucks_A.quantity_arrived);
						fflush(stdout);

						
					}

					else{
						p1=0;
						truck_A_arrive_S1=0;
						storage_S1.move_A=0;
					}
	    		}
    		
	    		/* In case needed to import new matarial  from  B  
				=====================================================*/
				if(!truck_A_arrive_S1){
					
					if(storage_S1.number_of_truck_nedded_B > 0  && !truck_B_arrive_S1){
						storage_S1.move_B=1; work=1;
						#ifdef __PRINT1__
						printf("number_of_truck_nedded_B: %d \n\n",storage_S1.number_of_truck_nedded_B );
						fflush(stdout);
						#endif

						/* numb. of trucks that will leave factory B (loaded with matarial B )*/
						                                                    
						trucks_B.trucks_departing= (storage_S1.number_of_truck_nedded_B >= number_of_B_trucks ? number_of_B_trucks:
						                                                                                storage_S1.number_of_truck_nedded_B );
												      
					
						/*Print just for the first time*/
						if(p1){
							printf("%s  %d  Trucks leave Factory B  ", RED, trucks_B.trucks_departing );
							fflush(stdout);
							/* Time needed to arrive S1 */
							for(int j=0; j< 6; j++){
								for(int k=0; k< 100000000; k++);
								printf(". ");
								fflush(stdout);
							}
							printf("\r%s  >> %d Trucks loaded with B arrived  Detergent factory <<                 \n  " ,CYAN, trucks_B.trucks_departing );
							fflush(stdout);
							printf("%s\n" ,WHITE);
							fflush(stdout);

							
						}

						truck_B_arrive_S1=1;

						/* calculate numb. of materials B arrived */
						trucks_B.quantity_arrived = trucks_B.trucks_departing *  capacity_of_B_truck;
						printf("\n\n >> quantity_of_B_arrived :%d                               \n" , trucks_B.quantity_arrived);
						fflush(stdout);
 
		    		}
		    		else{
						p1=0;
		    		    truck_B_arrive_S1=0;
						storage_S1.move_B=0;
		    		}  		
	    		}
				
         	}
			if(my_id == 0 && storage_S1.number_of_truck_nedded_B == 0 && storage_S1.number_of_truck_nedded_A == 0){
				work=0;
			}
			/* L1 needs to unloaded trucks and store materials in S1 */  
			
			/* In Case Truck loaded with A arrives 
			=========================================*/
			if(truck_A_arrive_S1){
				work=1;
				while(trucks_A.quantity_arrived > 0){
					
					/* L1 unloaded material A from truck */
					pthread_mutex_lock (&Truck_A_mutex ); 
					
					
					L1_carried= (trucks_A.quantity_arrived >= L1_carried_quantity ?  L1_carried_quantity:
																				trucks_A.quantity_arrived);			

					trucks_A.quantity_arrived -= L1_carried;
					printf("%s [%d] moves %d from A to S1 Still %d %s                                  \n" ,GREEN, my_id, L1_carried, trucks_A.quantity_arrived, WHITE );
					fflush(stdout);
					
					pthread_mutex_unlock (&Truck_A_mutex );

					/*Time nedded to move items */
					sleep(2);

					pthread_mutex_lock (&S1_mutex); 
					storage_S1.quantity_of_A_product_in_S += L1_carried;
					printf("%s [%d] add %d  to A in S1 , A becomes %d %s                            \n" ,YELLOW, my_id, L1_carried, storage_S1.quantity_of_A_product_in_S, WHITE );
					fflush(stdout);
					pthread_mutex_unlock (&S1_mutex);

					/*Time nedded to retern to truck and carried items */
					sleep(1);

				}
				truck_A_arrive_S1=0;
			}
          
			/* In Case Truck loaded with B arrives 
			============================================*/
			if(truck_B_arrive_S1){

				while(trucks_B.quantity_arrived > 0){
					/* L1 unloaded material A from truck */
					pthread_mutex_lock (&Truck_B_mutex ); 
					
					L1_carried= (trucks_B.quantity_arrived >= L1_carried_quantity ? L1_carried_quantity:
																				trucks_B.quantity_arrived );

					trucks_B.quantity_arrived -= L1_carried;
					printf("%s [%d] moves %d from B to S1 Still %d %s                      \n" ,GREEN, my_id, L1_carried, trucks_B.quantity_arrived, WHITE );
					fflush(stdout);
					pthread_mutex_unlock (&Truck_B_mutex );

					/*Time nedded to move items */
					sleep(3);

					pthread_mutex_lock (&S1_mutex); 

					storage_S1.quantity_of_B_product_in_S += L1_carried;
					printf("%s [%d] add %d  to B in S1 , B becomes %d %s                     \n" ,YELLOW, my_id, L1_carried, storage_S1.quantity_of_B_product_in_S, WHITE );
					fflush(stdout);

					pthread_mutex_unlock (&S1_mutex);


					/*Time nedded to retern to truck and carried items */
					sleep(2);

				}
				truck_B_arrive_S1=0;
			}                    
		}
		sleep(3);
			
	}

}


/* L2 Needs to move A and B materials to the manufacturing lines 
==================================================================*/

void move_materials_manufacturing_lines(void* id){

    int my_id = *((int*)id);
    

    int counter=0;
    int need=0, line_need;
    /*  2/3 of the amount L2 carries from A, while the rest if from B */
	int L2_carries_from_A= (int)(floor((2/3.0) * L2_carried_quantity ));
	int L2_carries_from_B= (int)(floor((1/3.0) * L2_carried_quantity ));

    while(produce){ 

		/* If the quantity of materials exceeds the minimum , and 
		the employee does not unload the materials from the truck.*/
		if(!truck_A_arrive_S1 && !truck_B_arrive_S1 && my_id==0){
			
			pthread_mutex_lock (&S1_mutex);	
			if(storage_S1.quantity_of_A_product_in_S <= starage_limit_A || 
			   storage_S1.quantity_of_B_product_in_S <= starage_limit_B){
				#ifdef __PRINT1__
        		printf("HEREEEEEEEEEEEEEEEEEEEEEEEEE\n");
				fflush(stdout);
				#endif
        		pthread_mutex_unlock (&S1_mutex);
        		pthread_cond_broadcast(&fill_S1_A);
        	 
        	}
			pthread_mutex_unlock (&S1_mutex);
		}



		/* If there is enough materials in S1 to carried by L2 */
		/*  2/3 of the amount L2 carries from A, while the rest if from B */
		pthread_mutex_lock (&S1_mutex);
		if(storage_S1.quantity_of_A_product_in_S >= L2_carries_from_A && 
		   storage_S1.quantity_of_B_product_in_S >= L2_carries_from_B  ){
			   pthread_mutex_unlock (&S1_mutex);
			/* Disribute the materials on production lines in order*/
			/* So, L2 choose for which production line he want to move the materials to */

			/* To cheack production lines in order */
			pthread_mutex_lock (&Manufacturing_Line_conter_Mutex);
			counter = manufacturing_Line.conter_lines_A;

			need=0;
			/* if line number (counter) needs material A (at least must be 4A, 2B)*/
			if( (2* material_first_technical_nedded_from_A) >= manufacturing_Line.A_in_line[counter]){
				
				need=1;
				line_need = counter;
				#ifdef __PRINT1__
				printf("[%d] production line %d need materials\n ",my_id, line_need);
				fflush(stdout);
				#endif
		
			}
			manufacturing_Line.conter_lines_A= (manufacturing_Line.conter_lines_A+1) < NUMBER_OF_PRODUCTION_LINE ? 
												manufacturing_Line.conter_lines_A + 1 : 0 ;
			
			pthread_mutex_unlock (&Manufacturing_Line_conter_Mutex);
			/* Just to unlock mutex quickly*/
			if(need){
				
				pthread_mutex_lock(&production_line_mutex[line_need]);
				/* if the production line needs material*/
				
				pthread_mutex_lock (&S1_mutex);
				storage_S1.quantity_of_A_product_in_S -= L2_carries_from_A ;
				storage_S1.quantity_of_B_product_in_S -= L2_carries_from_B ;
				pthread_mutex_unlock (&S1_mutex);
				/*time needed to move material to production line */
				//sleep(3);
				
				manufacturing_Line.A_in_line[line_need]+= L2_carries_from_A;
				manufacturing_Line.B_in_line[line_need]+= L2_carries_from_B;
				#ifdef __PRINT1__
				printf(" [%d] A in %d line is %d, B is %d \n ",my_id, line_need, manufacturing_Line.A_in_line[line_need], manufacturing_Line.B_in_line[line_need]);
				fflush(stdout);
				#endif
				pthread_mutex_unlock(&production_line_mutex[line_need]);
				/*time to return back to S1 */
				sleep(3);
				need=0;
				
			}

		} 
		pthread_mutex_unlock (&S1_mutex);




    	sleep(4);
	
    }
    

}


/* Start production proccessing 
===================================*/
void production_line_process (void* id){



	int line = *((int*)id);
	#ifdef __PRINT2__
	printf(" Production %d created \n",(line+1));
	fflush(stdout);
	#endif

	/* Creat first technical employee */
	if(pthread_create(&first_technical[line], NULL, (void *)
		first_technical_employee, (void*)&line) != 0){
		perror("Error: Create first technical employee \n ");
		exit(7);
	} 


/*	
	struct tecnical_employee employee[NUMBER_OF_TECH_EMPLOYEE];

	for (int i=1 ; i< (NUMBER_OF_TECH_EMPLOYEE -1); i++){
		employee->id = i;
		employee->line= line;
		employee->time= emp_speed[i]; 

		if(pthread_create(&emp_thread[i],NULL, (void *)
			employee_steps, (void*)&employee[i]) != 0){
			perror("Error: Create technical employees \n ");
			exit(8);
		} 
	}
*/

	/* Creat last technical employee */
	if(pthread_create(&last_technical[line], NULL, (void *)
		last_technical_employee, (void*)&line) != 0){
		perror("Error: Create last technical employee \n ");
		exit(9);
	} 


	/* wait threads until finish there work*/
	if (pthread_join( first_technical[line], NULL)!= 0){
		perror("Error: Pthread join first technical employee ");
		exit(10);
	}

/*
	for (int i=1 ; i< NUMBER_OF_TECH_EMPLOYEE-1; i++){
		if (pthread_join( emp_thread[line], NULL)!= 0){
			perror("Error: Pthread join technical employees ");
			exit(11);
		}
	}
    
*/

	/* wait threads until finish there work*/
	if (pthread_join( last_technical[line], NULL)!= 0){
		perror("Error: Pthread join last technical employee ");
		exit(12);
	}



}



/* The first technical employee 
===============================*/
void first_technical_employee (void* arg){
	int line =  *((int*)arg);
	int process_time = (rand() % (Max_speed - Min_speed + 1)) + Min_speed;
	while (produce){
		sleep(process_time);
		pthread_mutex_lock(&S2_mutex);
		/* If */
		if(storage_S2.number_of_boxes >= starage_Max_S2){

			pthread_mutex_unlock(&S2_mutex);
			sleep(3);
		}
		else{

			pthread_mutex_unlock(&S2_mutex);
			if (manufacturing_Line.A_in_line[line] >= material_first_technical_nedded_from_A 
			&& manufacturing_Line.B_in_line[line] >= material_first_technical_nedded_from_B ){
				

				manufacturing_Line.A_in_line[line]-= material_first_technical_nedded_from_A;
				manufacturing_Line.B_in_line[line]-= material_first_technical_nedded_from_B;
				printf("%s First technical employee in line %s %d %s Start producing %s\r",RED,YELLOW,line,RED,WHITE);
				fflush(stdout);
				/* Send signal to the last teqnical employee */
				pthread_cond_signal(&emplyee_cond[line][NUMBER_OF_TECH_EMPLOYEE-1]);

			}
		}

	}
	

}



void employee_steps(void* arg){ 
	int line =  *((int*)arg);



	//pthread_cond_signal(&emplyee_cond[line][NUMBER_OF_TECH_EMPLOYEE-1]);


/*
	struct tecnical_employee employee= *((tecnical_employee*)&arg);
	


	printf(" CREATED id: %d, time: %d, line: %d \n",employee.id, employee.time, employee.line);
	fflush(stdout);

*/
}


/* The last technical employee
=================================*/
void last_technical_employee (void* arg){
	int line =  *((int*)arg);
	int process_time = (rand() % (Max_speed - Min_speed + 1)) + Min_speed;
	while (produce){
		#ifdef __PRINT2__
		printf("%s[Last]: from line %d waiting signal ...%s\n",CYAN,line, CYAN);
		fflush(stdout);   
		#endif    
		pthread_cond_wait(&emplyee_cond[line][NUMBER_OF_TECH_EMPLOYEE-1], &production_line_mutex[line]);
		printf("\r%s[Last]: from line %d << WORKING >> ...%s                               \r",CYAN,line, WHITE);
		fflush(stdout);  

		/* >> WORKING  << */
		sleep(process_time);
		/* Count the number of manufactured products and put it in boxes */
		pthread_mutex_lock(&Manufacturing_Line_product_Mutex);
		manufacturing_Line.manufac_product[line]+=1;
		/* If the Box is full */
		if(manufacturing_Line.manufac_product[line] >= Size_Of_Box){

			manufacturing_Line.manufac_product[line] -= Size_Of_Box;
			manufacturing_Line.manufac_boxes[line]+=1;
			printf("\n%s  BOX IS FULL  [%d]...%s                                 \n\n",YELLOW,line, WHITE);
			fflush(stdout); 
			pthread_cond_broadcast(&L3_cond);


		}
		pthread_mutex_unlock(&Manufacturing_Line_product_Mutex);
		#ifdef __PRINT1__
		printf("\n%s  LINE  [%d]  PRODUCT %d ...%s\n\n",YELLOW,line,manufacturing_Line.manufac_product[line], YELLOW);
		fflush(stdout); 
		#endif


	}

}



/* L3 moves boxes to trucks 
===============================*/
void move_boxes_to_trucks(void* id){

	int counter=0, move_to_S2=0 , line;
	int my_id = *((int*)id);

	while(produce ){
		
		/* wait signals from last employee  */
		pthread_mutex_lock ( &Manufacturing_Line_conter_boxes_Mutex);
		pthread_cond_wait( &L3_cond, &Manufacturing_Line_conter_boxes_Mutex);
		/* To cheack production lines in order */
		counter = manufacturing_Line.conter_last_step;

		/* If the line has full box move it to the truck else move to the next line */
		if (manufacturing_Line.manufac_boxes[counter] > 0){

			manufacturing_Line.manufac_boxes[counter]-=1;	
			line= counter;
			move_to_S2=1;
		}

		manufacturing_Line.conter_last_step= (manufacturing_Line.conter_last_step+1)< NUMBER_OF_PRODUCTION_LINE? 
													manufacturing_Line.conter_last_step+1 : 0;
	
		pthread_mutex_unlock (&Manufacturing_Line_conter_boxes_Mutex);


		if(move_to_S2){

			/*time to move products*/
			printf("[%d]: Move Box from line %d to S2                                \n ",my_id, line);
			fflush(stdout);
			sleep(2);

			/* add boxes to S2*/
			pthread_mutex_lock (&S2_mutex);
			storage_S2.number_of_boxes +=1;
			number_of_box_to_manufacture -=1;
			pthread_mutex_unlock (&S2_mutex);

			
			pthread_cond_broadcast(&detergent_cond);



		
			move_to_S2 =0;
		}

	}
	/*
	if(produce &&  number_of_box_to_manufacture == 0){
		print_finaly();
		
		exit(0);
	}*/

}



/*detergent factory employee*/
void detergent_factory_employee (void* id){
	int my_id = *((int*)id);
	int move_to_truck=0; 
	int counter=0;
	while (produce){
		/* if there are boxes in s2 */
		pthread_mutex_lock(&S2_mutex);
		if(storage_S2.number_of_boxes > 0){


			/* If there are available trucks */
			pthread_mutex_lock(&Truck_product_mutex);
			counter= Truck_product.counter;
			if(Truck_product.truck_avalabile[counter]){
				
				storage_S2.number_of_boxes-=1;
				pthread_mutex_unlock(&S2_mutex);
				move_to_truck=1;

			}
			pthread_mutex_unlock(&Truck_product_mutex);




			if(move_to_truck){
				printf("\n%s[%d]: products in truck [ befor ]%d ...                                %s\n",YELLOW,my_id, Truck_product.number_of_boxes, WHITE);
				fflush(stdout);  
				/* Time needed to move boxes to truck*/
				sleep(2);

				/* add boxes to truck */
				pthread_mutex_lock(&Truck_product_mutex);
				Truck_product.number_of_boxes+=1;
				if(Truck_product.number_of_boxes >= Truck_product_size){
					Truck_product.truck_avalabile[counter]=0;
					/*time to leave and return back */
					printf("\n\n Truck Leaving ...                              \n\n");
					fflush(stdout);
					sleep(Trip_Time);
					Truck_product.truck_avalabile[counter]=1;
				}
				pthread_mutex_unlock(&Truck_product_mutex);
				move_to_truck=0;


				printf("%s[%d]: products in truck [ after ]%d ...%s                             \n\n",YELLOW,my_id, Truck_product.number_of_boxes, WHITE);
				fflush(stdout);  
			}

			

		}
		/* else wait signal from L3 */
		else{
			pthread_mutex_unlock(&S2_mutex);

			pthread_mutex_unlock(&Truck_product_mutex);
			pthread_cond_wait( &detergent_cond, &Truck_product_mutex);
			pthread_mutex_unlock(&Truck_product_mutex);
			
		}

		sleep(3);
			



	}
	

}







void print_finaly(){
	printf("EXIT !                          \n");

}