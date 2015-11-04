#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/sem.h>

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

void *proc1 (int semid) {
	//printf("thread 1 : create\n");
	int i;
	for(i='A';i<'Z'+1;i++){
		prendre(semid, 0);
		printf("%c",i);
		fflush (stdout);// flushing or repositioning required
		rendre(semid, 1);
	}
	pthread_exit(0);
}

void *proc2 (int semid) {
	//printf("thread 2 : create\n");
	int i;
	for(i='a';i<'z'+1;i++){
		prendre(semid, 1);
		printf("%c",i);
		fflush (stdout);// flushing or repositioning required
		rendre(semid, 0);
	}
	pthread_exit(0);
}

int main(int argc, char argv[]) {
	int pid1, pid2;
	int ret1,ret2;
	int res1 = 0;
	int res2 = 0;
	//Semaphore
	key_t cle;
	int er;
	cle = ftok("/home/eleves/promo16/elec/fan/Documents/embeddedLinux/tp3/tp3_prog2.log",0);
	if(cle == (key_t) -1) {
		perror("erreur ftok\n");
		exit(1);
	}
	int semid;
	semid = semget(cle,2,IPC_CREAT|0666);
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
	
	arg.val = 0;
	er = semctl(semid,1,SETVAL,arg);
	if (er == -1) {
		perror("erreur semctl\n");
		exit(1);
	}
		
	pid1 = fork();
	if(pid1 == 0) proc1 (semid);
	pid2 = fork();
	if(pid2 == 0) proc2 (semid);
	wait (&ret1); //Il attend que le thread finit son travail
	wait (&ret2); //Il attend que le thread finit son travail
	
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


