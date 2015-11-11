#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1,mutex2 ;

void *thread1(void *arg){
	int c;
	for(c='a';c<='z';c++){
	
		pthread_mutex_lock(&mutex1);

		printf("%c",c);

		pthread_mutex_unlock(&mutex2);
	}
	pthread_exit(0);
}

void *thread2(void *arg){
	int c;
	for(c='A';c<='Z';c++){
	
		pthread_mutex_lock(&mutex2);

		printf("%c",c);
	
		pthread_mutex_unlock(&mutex1);
	}
	pthread_exit(0);
}


int main (char ** argv, int argc){
	pthread_t th1, th2;	
	
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	
	pthread_mutex_lock(&mutex1);
	
	pthread_create(&th1, NULL, thread1, NULL);	
	pthread_create(&th2, NULL, thread2, NULL);
	
	pthread_join (th1, NULL); 
	pthread_join (th2, NULL);
	
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
}
