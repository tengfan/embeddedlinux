#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h> 
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END	0
#define WRITE_END	1

void rendre(int semid, int no){
	struct sembuf op[1];
	int er;
	op[0].sem_num = no;	// semaphore no
	op[0].sem_op = 1;	// liberation (set -1 to take)
	op[0].sem_flg = 0;	// blocked operation
	er = semop(semid,op,1);
	if (er == -1) {
		printf("no: %d \n",no);
		perror("erreur semop rendre\n");
		exit(1);
	}
}

void prendre(int semid, int no){
	struct sembuf op[1];
	int er;
	op[0].sem_num = no;	// semaphore number
	op[0].sem_op = -1;	// liberation (set -1 to take)
	op[0].sem_flg = 0;	// blocked operation
	er = semop(semid,op,1);
	if (er == -1) {
		printf("no: %d \n",no);
		perror("erreur semop prendre\n");
		exit(1);
	}
}

void *proc1 (int tube[2], int semid) {
	char cmd[50];
	int file_rd=fileno(stdin);  /* filedescriptor = 0 */
	int file_wr=fileno(stdout); /* filedescriptor = 1 */
	int l;
	do{
		prendre(semid, 0);
		dup2(tube[WRITE_END], file_wr);
		close(tube[READ_END]);
		fgets(cmd, 50, stdin);
		l=strlen(cmd);
		write(tube[WRITE_END], cmd, l);
		rendre(semid,1);
	}while(strcmp(cmd,"quit\n")!=0);
	close(tube[WRITE_END]);
	exit(0);
}

void *proc2 (int tube[2], int semid) {
	char cmd[50];
	int file_rd=fileno(stdin);  /* filedescriptor = 0 */
	int file_wr=fileno(stdout); /* filedescriptor = 1 */
	int l;
	int i;
	do{
		prendre(semid, 1);
		dup2(tube[READ_END], file_rd);
		close(tube[WRITE_END]);

		l = read(tube[READ_END], cmd, 50);
		for(i=0;i<l-1;i++){
			cmd[i] = toupper(cmd[i]);
		}
		cmd[l-1] = '\0';
		printf("%s\n", cmd);
		rendre(semid, 0);
	}while(strcmp(cmd,"QUIT\n")!=0);
	close(tube[READ_END]);
    exit(0);
}

int main(int argc, char argv[]) {
	//Tube initialise
	int tube[2];
	pipe(tube);
	int pid;
	int ret = 0;
	key_t cle;
	int er;
	int semid;
	semid = semget(cle,2,IPC_CREAT|0666);
	if(semid == -1) {
		perror("semget\n");
		exit(1);
	}
	
	union semun{
		int val;
		struct smid_ds *buf;
		ushort *array;
	} arg;
	arg.val = 1;
	er = semctl(semid,0,SETVAL,arg);
	if (er == -1) {
		perror("semctl\n");
		exit(1);
	}
	
	arg.val = 0;
	er = semctl(semid,1,SETVAL,arg);
	if (er == -1) {
		perror("semctl\n");
		exit(1);
	}

	pid = fork();
	if(pid == 0) {
		proc2 (tube, semid);
	}
	proc1(tube, semid);
	wait (&ret);
	
	//Once finished, it clears the semaphore
	if (semctl(semid, 1, IPC_RMID ) == -1 )
	{
	  perror("semctl failure while clearing Reason:");
	  exit(-1);
	}
	return 0;
}


