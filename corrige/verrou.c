#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include <pthread.h>
#include <unistd.h>

//a compiler avec gcc verrou.c -lpthread

//Déclaration de deux mutexs afin de passaer la main alternativement entre les deux threads
pthread_mutex_t mutex,mutex2 ;

//Thread qui écrit les minuscules
void *my_thread_process(void *arg){
	int c;
	for(c='a';c<='z';c++){
	
		pthread_mutex_lock(&mutex);

		printf("%c\n",c);

		pthread_mutex_unlock(&mutex2);
	}
	pthread_exit(0);
}

//Thread qui écrit les majuscules
void *my_thread_process2(void *arg){
	int c;
	for(c='A';c<='Z';c++){
	
		pthread_mutex_lock(&mutex2);

		printf("%c\n",c);
	
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}


int main (char ** argv, int argc){
	//Déclaration des deux mutex
	pthread_t th1, th2;	
	
	//initialisation des mutex
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);
	
	pthread_mutex_lock(&mutex); // Pour commencer par le bon thread (les majuscule)
	
	//création des deux threads
	pthread_create(&th1, NULL, my_thread_process, NULL);	
	pthread_create(&th2, NULL, my_thread_process2, NULL);
	
	//execution des threads		
	pthread_join (th1, NULL); 
	pthread_join (th2, NULL);
	
	//Destruction des mutex
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
}
