#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h> 
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
	char c;
    int shmid;
    key_t key;
    char *shm, *s;
	char shm_init = 27;

    /*
     * la cle de la memoire partagee = "1234".
     */
    key = 1234;

    /*
     * Creer le segment.
     */
    if ((shmid = shmget(key, shm_init, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

	/*
	 * Attacher le segment de memoire partagee
	*/
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    s = shm;
	prendre(semid, 0);
    for (c = 'A'; c <= 'Z'; c++) *s++ = c;
	rendre(semid,1);

	*s = NULL;
	
	exit(0);
}

void *proc2 (int semid) {
	int shmid;
    key_t key;
    char *shm, *s;
	char shm_init = 27;

    key = 1234;

    /*
     * trouver le segment.
     */
    if ((shmid = shmget(key, shm_init, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     *Attacher le segment de la memoire partagee
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Demande le continu dans la memoire partagee
     */
	prendre(semid, 1);
    for (s = shm; *s != NULL; s++) {
		*s = tolower(*s);
		//On transfert les majuscules en miniscules dans la memoire partagee
		*shm = tolower(*s);
		shm++;
		printf("%c",*s);
	}
	printf("\n");
	rendre(semid, 0);

    exit(0);
}

int main(int argc, char argv[]) {
	int pid1, pid2;
	int ret1 = 0;
	int ret2 = 0;
	key_t cle;
	int er;
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
	
	//Quand on a fini, on nettoye le semaphore
	if (semctl(semid, 1, IPC_RMID ) == -1 )
	{
	  perror("semctl failure while clearing Reason:");
	  exit(-1);
	}
	return 0;
}


