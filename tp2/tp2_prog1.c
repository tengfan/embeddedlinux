#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

FILE *pFile;
void *thread1 (void *p) {
	printf("thread 1 : create\n");
	int i = 0;
	for(i=0;i<100001;i++){
		fprintf(pFile,"%d\n",i);
	}
	pthread_exit(0);
}

void *thread2 (void *p) {
	printf("thread 2 : create\n");
	int i = 0;
	for(i=100000;i<1100001;i++){
		fprintf(pFile,"%d\n",i);
	}
	pthread_exit(0);
}

int main(void) {
	pthread_t t1, t2;
	int ret1,ret2;
	int res1 = 0;
	int res2 = 0;
	pFile = fopen ("tp2_prog1.log" , "w");
	ret1 = pthread_create(&t1,NULL,thread1,"1");
	if(ret1 != 0){
		perror("Pb creation du thread\n"); exit(0);
	}
	
	ret2 = pthread_create(&t2,NULL,thread2,"2");
	if(ret2 != 0){
		perror("Pb creation du thread\n"); exit(0);
	}
	
	pthread_join(t1, &res1);
	pthread_join(t2, &res2);
	fclose(pFile);
}


