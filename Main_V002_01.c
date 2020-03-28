/*
Go through ReadMe file to checkout purpose of this file.
One must run this program with mentioned command
cc file.c -std=c99 -lpthread

<Version> tkl_001_01
Next version tkl_100_01 Check file: HISTORY
Author: Vivek Kumar Sahu
Date:   28/03/2020

*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAX_USER 65
#define MAX_SEAT 64
#define MAX_NAME_LENGTH 18
#define Log_NAME "TKL_SAHU"
#define Log_NAME_LENGTH strlen(Log_NAME)

sem_t window;
int seatNo = MAX_SEAT;
pthread_t timer;
pthread_mutex_t mutex;
bool open = false;

void* traveller(void);
int getTicket(unsigned int traveller_ID);
void*countDownTimer(int countDown);
bool isWindowOpened(void);
bool isSeatsAvailable(void);

main(){
	pthread_t pid[MAX_USER];

	/* Value '0' to start semaphore with as we must not open the ticketing window before given time */
	sem_init(&window, 0, 0);

	for(int i=0; i< MAX_USER ; i++){

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

	unsigned int traveller_ID = (unsigned int)pthread_self();

	char traveller_name[MAX_NAME_LENGTH + 1] = {'\0'};

	sprintf(traveller_name, "%u",traveller_ID);
	strcat(traveller_name, Log_NAME);
	printf("traveller_ID : %u\n", traveller_ID);

	if(isSeatsAvailable()){
		printf("traveller_name : %s seatNo:%d\n",traveller_name,getTicket((unsigned int)pthread_self()));
	}
        else{
		printf("traveller_ID : %u No Seats Available\n",traveller_ID);
	}
}

int getTicket(unsigned int traveller_ID){
	int _seat = 0;

	pthread_mutex_lock(&mutex);
	if(isWindowOpened()){
		printf("Window is opened\n");

		_seat = seatNo--;
                printf("traveller_ID : %u seatNo:%d\n",(unsigned int)traveller_ID,_seat);
	}
	pthread_mutex_unlock(&mutex);

return _seat;
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

bool isSeatsAvailable(void){
	bool available = false;
	/*
	  Use same mutex what we are using while allocating the seats. mutex prevents race condition
	*/
	pthread_mutex_lock(&mutex);
	(seatNo > 0) ? (available = true) : (available= false);
	pthread_mutex_unlock(&mutex);

return available;
}


