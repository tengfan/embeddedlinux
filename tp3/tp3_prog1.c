#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/sem.h>

FILE *pFile;
int val;
int semid;
void *thread1 (void *p) {
	printf("thread 1 : create\n");
	int i;
	prendre(semid, 0);
	for(i=0;i<100;i++){
		val++;
	}
	rendre(semid, 0);
	pthread_exit(0);
}

void *thread2 (void *p) {
	printf("thread 2 : create\n");
	int i;
	prendre(semid, 0);
	for(i=100;i<200;i++){
		val++;
	}
	rendre(semid, 0);
	pthread_exit(0);
	//semctl(semid,0,IPC_RMID); //removing the 0th semaphore in the set
}

//Fonction permettant de rendre le semaphore no de l'ensemble semid
void rendre(int semid, int no){
	struct sembuf op[1];
	int er;
	op[0].sem_num = no;	// semaphore no
	op[0].sem_op = 1;	// liberation (a mettre a -1 pour une prise)
	op[0].sem_flg = 0;	// operation bloquante
	er = semop(semid,op,1);
	if (er == -1) {
		printf("no: %d \n",no);
		perror("erreur semop rendre\n");
		exit(1);
	}
}

//Fonction permettant de prendre le semaphore
void prendre(int semid, int no){
	struct sembuf op[1];
	int er;
	op[0].sem_num = no;	// semaphore number
	op[0].sem_op = -1;	// liberation (a mettre a -1 pour une prise)
	op[0].sem_flg = 0;	// operation bloquante
	er = semop(semid,op,1);
	if (er == -1) {
		printf("no: %d \n",no);
		perror("erreur semop prendre\n");
		exit(1);
	}
}
int main(int argc, char ** argv) {
	pthread_t t1, t2;
	int ret1,ret2;
	int res1 = 0;
	int res2 = 0;
	//Semaphore
	key_t cle;
	int er;
	val = 0;
	cle = ftok(argv[0],0);
	if(cle == (key_t) -1) {
		perror("erreur ftok\n");
		exit(1);
	}

	semid = semget(cle,1,IPC_CREAT|0666);
	if(semid == -1) {
		perror("erreur semget\n");
		exit(1);
	}
	
	//Initialisation a 1 de notre semaphore (qui est le semaphore number 0 de l'ensemble)
	union semun{
		int val;
		struct smid_ds *buf;
		ushort *array;
	} arg;
	arg.val = 1;
	er = semctl(semid,0,SETVAL,arg);
	if (er == -1) {
		perror("erreur semctl\n");
		exit(1);
	}
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
	
	printf("%d\n",val);
	
	//once done clear the semaphore set
	if (semctl(semid, 1, IPC_RMID ) == -1 )
	{
	  perror("semctl failure while clearing Reason:");
	  exit(-1);
	}
	//exit the main threads
	pthread_exit(NULL);
	return 0;
}


