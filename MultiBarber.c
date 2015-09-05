/*
 * MuiltiBarber.c
 * 
 * Copyright 2014 bhaskar <bhaskar@bhaskar-HP-Pavilion-g6-Notebook-PC>
 * Copyright BHASKARJYOTI DAS  24/09/2014
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include <stdlib.h>
#include <time.h>


#define NUM_CTHREADS	20   // number of customers
#define NUM_BTHREADS 	5   // number of barbers
#define SHOP_TIME  20000        // total time the SHOP IS OPEN.. this is NOT used
#define HAIR_CUT_TIME 1     //TIME TAKE FOR A HAIR CUT
#define WAITING_ROOM_SEATS  15

//#define LOG

char ch; 
time_t end_time; /*end time*/

sem_t customer,barber;/*Two semaphors for barber and customer to signal each other*/

pthread_mutex_t accessSeats;    //Mutex being used for the seats 

int NumberOfFreeSeats=WAITING_ROOM_SEATS ; /* this includes barber chamber's chair */
int NumberHairCutDone=0;
 
void *barber_function(void *arg);
void *customer_function(void *arg);


int main(int argc, char **argv)
{

   pthread_t customerid[NUM_CTHREADS], barberid[NUM_BTHREADS]; // an array of threads
   pthread_attr_t attr;

   int rc; //used as return for pthread_create
   long t; // used as void casted param to thread function  and is essentially thread array index
    	
	end_time=time(NULL)+SHOP_TIME;/*Barber Shop Hours is 30s.. we run this simulation for 30 s*/
	printf("...................SIMULATION PARAMETERS......................\n");
	printf("****************************************************************\n");
	printf("*MULTIPLE SLEEPING BARBER SIMULATION BY POSIX IPC               \n");
	printf("*by BHASKARJYOTI DAS  2014 MTech CSE BATCH                     *\n");
	printf("*Number of WAITING ROOM seats =%d                              *\n", WAITING_ROOM_SEATS);
	printf("*Number of customers = %d                                      *\n",NUM_CTHREADS);
	printf("*Number of Barbers = %d                                        * \n",NUM_BTHREADS);
	printf("****************************************************************\n");
	
	printf ("\n\n Program output is redirected to multibarberIPC.txt ..\n");
	printf ("\n\n If the #define LOG is commented in code, stdout will be used \n");

	printf ("\n\nPress a key to continue..\n");
	ch = getchar();
#ifdef LOG 
	FILE*newout = freopen("multibarberIPC.txt", "w", stdout);
	if (!newout) { perror("freopen"); exit (EXIT_FAILURE); } 
	stdout = newout;
#endif 	
	/*Semaphore  and mutex initialization*/
	pthread_mutex_init(&accessSeats, NULL);
	sem_init(&customer,0,0);  // not a shared semaphore and initilized to 0 
	sem_init(&barber,0,0);    // not a shared semaphore and initilized to 1 as there is only one barber available
	
	//sem_init(&chair,0,NumberOfFreeSeats); // for chairs, we use a semaphore again  */
	
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/*Barber_thread initialization */
	for(t=0;t<NUM_BTHREADS;t++)
		{
     printf("In main: creating Barber thread %ld\n", t);
     rc = pthread_create(&barberid[t], NULL, barber_function, (void *)t);
    
     if (rc){
		 perror("create barbers is failure!\n");	
         exit(-1);
			}
	printf("This barber is ready and shop is open :%d free seats  \n",NumberOfFreeSeats);
		}
	
	
	/*Customer_thread initialization.. there are 6 customers */

	for(t=0;t<NUM_CTHREADS;t++)
	{
     printf("In main: creating customer thread %ld\n", t);
     rc = pthread_create(&customerid[t], NULL, customer_function, (void *)t);
     if (rc){
		 perror("create customers is failure!\n");	
         exit(-1);
			}
	}


  /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   
	/*we join barber thread */
	
	for(t=0;t<NUM_BTHREADS;t++)
	{
     printf("In main: joining Barber thread %ld\n", t);
     rc = pthread_join(barberid[t], NULL);
	}
	
	/*we join customer thread */

	
	for(t=0;t<NUM_CTHREADS;t++)
	{
     printf("In main: joining customer thread %ld\n", t);
     rc = pthread_join(customerid[t], NULL);
	}
	pthread_mutex_destroy(&accessSeats);
	sem_destroy(&customer);
	sem_destroy(&barber);
#ifdef LOG 	
 fclose(newout);
#endif  
 printf ("Simulation over .. check the file multibarberIPC.txt  \n");
 printf ("We will exit from main \n");
 fflush(stdout);
 pthread_exit(NULL);
}


void *barber_function(void * threadid)
{ 

   long tid = (long)threadid;
   printf("Barber starting the day : barber thread #%ld!\n", tid);
   			
	while (NumberHairCutDone<NUM_CTHREADS)   // infinite loop till day is over
					{
		sem_wait(&customer);/*P(customers)*/            
		/* got a customer .. now update the free seat counter */
		pthread_mutex_lock (&accessSeats);
		NumberOfFreeSeats++;
		printf("Barber:got a customer , Now, number of free seats is:%d.\n",NumberOfFreeSeats);
		NumberHairCutDone++;  // update before releasing mutex
		pthread_mutex_unlock (&accessSeats);	
		sleep(HAIR_CUT_TIME);  
		printf ("Barber has done a hair cut ..\n");
		sem_post(&barber);/*V(barbers)*/
		printf("Number of Customer got the hair cut done:%d \n",NumberHairCutDone); 
		if (NumberHairCutDone==NUM_CTHREADS)
				break;  //In this case come out of infinite while loop
	
		}  // end of while loop 
// This part of the code will NOT come unless there is an exit condition instead of while (1)
printf("Barber announcing good bye ! Day is over ..retiring  \n");  
fflush(stdout);
pthread_exit(NULL);
} 

void *customer_function (void * threadid)

{

			long tid = (long)threadid;
			printf("Customer has arrived : Customer thread #%ld!\n", tid);
			pthread_mutex_lock (&accessSeats);
			if(NumberOfFreeSeats > 0)  //Check if the saloon is full or not 
					{
			NumberOfFreeSeats--;
			printf("Customer has taken a free waiting seat :current free seat count is:%d\n",NumberOfFreeSeats);
			//unlock the mutext for use by other customers and barbers and wait for barber
			pthread_mutex_unlock (&accessSeats);
			sem_post(&customer); /*  notify the barber ..customer is waiting ...V(customers)*/
			/*update done...just put the mutex lock let go*/
			sem_wait(&barber);/* wait on the barber now ...P(barbers)*/
		
				}
		else
			{
			/*V(mutex)*/
			/*If the number is full of customers,just put the mutex lock let go*/
			pthread_mutex_unlock (&accessSeats);
			printf("%d seats are free.....\n", NumberOfFreeSeats);
			}
	
	pthread_exit(NULL);
} 
