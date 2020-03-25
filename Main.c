/*
Go through ReadMe file to checkout purpose of this file.
One must run this program with mentioned command
cc a.out -std=c99 -lpthread

<Version> tkl_001_01
Next version tkl_002_01 All ToDo to be implemented
Author: Vivek Kumar Sahu
Date:   25/03/2020

*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <stdlib.h>
#include<time.h> 
#include <stdbool.h>

#define MAX_USER 64
#define MAX_NAME_LENGTH 18
#define Log_NAME "TKL_SAHU"
#define Log_NAME_LENGTH strlen(Log_NAME)

sem_t window;
int seatNo = MAX_USER;
pthread_t timer;
pthread_mutex_t mutex;
bool open=false;

void* traveller(void);
int getTicket(unsigned int traveller_ID);
void*countDownTimer(int countDown);
bool isWindowOpened(void);

main(){
	pthread_t pid[MAX_USER];
        /* ToDo: create a valid traveller name
	char traveller_name[MAX_NAME_LENGTH + 1] = {'\0'};
	char travellerID[MAX_NAME_LENGTH - Log_NAME_LENGTH] ;//= {'\0'};
	*/

	
	/* Value '0' to start semaphore with as we must not open the ticketing window before given time */
	sem_init(&window, 0, 0);

	for(int i=0; i< MAX_USER ; i++){
		//strcpy(traveller_name, Log_NAME);
		//int j = rand();
		//sprintf(travellerID, "%d",j);
		//printf("travellerID = %s -> %d\n",travellerID,j);
		//traveller_name[MAX_NAME_LENGTH - strlen("sahu")] = '\0';

		pthread_create(&pid[i], 0 , (void*)traveller,0);
	}

	/* Create a counter timer starting from 10 till 0 */
	pthread_create(&timer, 0, (void*)countDownTimer, 10);

	for(int i=0; i< MAX_USER ; i++){
		pthread_join(pid[i],0);
	}

	pthread_join(timer,0);
}

void* traveller(void){

	/* ToDo We must print seatNo allocated here */ 
	(void)getTicket((unsigned int)pthread_self());
}

int getTicket(unsigned int traveller_ID){

	//sem_wait(&window); // deadlock solution hence isWindowOpened() Fn implement

	pthread_mutex_lock(&mutex);
	if(isWindowOpened()){
		printf("Window is opened\n");
		//Todo: Deadly deadlock here once we place mutex lock here. Check Reason 
		printf("traveller_ID : %u seatNo:%d\n",(unsigned int)traveller_ID,seatNo --);
	}
	pthread_mutex_unlock(&mutex);
}

bool isWindowOpened(void){

// remove open flag and see what happens
	while(1 && !open){
		sem_wait(&window);
		open=true;
		break;
	}

puts("Reservation Window is opened for ticketing");
return true;
}

void*countDownTimer(int countDown){

	while(countDown){
		printf("time remaining to get window open:%d s\n",countDown--);
	}
	sem_post(&window);
}


