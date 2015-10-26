#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
/* Partie 1 */
/*
void func(char nom[], char para[]){
	char chemin[20];
	sprintf(chemin, "/bin/%s", nom);
	execl(chemin, nom, para, NULL);
}

int main(void) {
	char nom[20];
	char para[5];
	int pid, ret;
	char logout[] = "logout";
	char para0[] = "0";
	while(1){
		printf("Commande > ");
		scanf("%s %s",nom, para);
		if(!strcmp(nom,logout) && !strcmp(para,para0)) exit(-1);
		
		pid = fork();
		if(pid == 0) func(nom,para);
		wait (&ret); //Il attend que le thread finit son travail
	}
	return 1;
}*/


/*Partie 2 */
// execv, strtok, fgets

int main(void) {
	int pid, ret;
	FILE * pFile;
	char str[30];
	char logout[] = "logout";
	char para0[] = "0";
	char chemin[20];
	char * tabArg;
	char * para[3];
	int error = 0;
	pFile = fopen ("commandes.sh" , "r");
	while(1){
		if(fgets(str, 1000, pFile) == NULL) {
			perror ("Error opening file");
			exit(-1);
		}
		else {
			pid = fork();
			if(pid == 0) {
				tabArg = strtok(str," \r\n");
				sprintf(chemin, "/bin/%s", tabArg);
				//Premier element
				para[0] = tabArg;
				//Essayer d'avoir le deuxieme element
				tabArg = strtok(NULL, " ");
				para[1] = tabArg;
				printf("%s\n", para[1]);
				para[2] = NULL;
				if(!strcmp(para[0],logout) && !strcmp(para[1],para0)){
					printf("logout\n");
					exit(-1);
				} 
				else{
					printf("bash > \n");
					error = execv(chemin, para);
					if(error != 0) printf("Error: %d\n",error);
				}
			}
			wait (&ret); //Il attend que le thread finit son travail
		}
	}
	
	fclose(pFile);
	return 1;
}
