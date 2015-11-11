#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//a compiler avec gcc tube.c -lpthread

int main(void)
{
	int i;
    	pid_t pid_fils;
    	int Tube1[2];
    	int Tube2[2];
    	
    	unsigned char message[50];
    	unsigned char messageMaj[50];
    	unsigned char message_a_convertir[50];
    	unsigned char message_converti[50];

	// Création du tube 1
    	pipe(Tube1);
    	
    	// Création du tube 2
    	pipe(Tube2);


	int l; //longueur de la chaine de caractère

	pid_fils = fork();

	//Procésus fils: fonction bc
	if(pid_fils == 0){
		//redirection de des entréé et sorti standard
		dup2(Tube1[0],0);
		dup2(Tube2[1],1);
		close(Tube1[1]);
		close(Tube2[0]);  
		
		//le procesus fils est "recouvert" par bc:
		execl("/usr/bin/bc","bc",NULL);
		
		close(Tube1[0]);
		close(Tube2[1]);
	}

	else{
	    	close(Tube1[0]);
		close(Tube2[1]); 
		printf("Entrez une opération à réaliser\n");
		fgets(message, 50, stdin);
		
		l=strlen(message);
		write(Tube1[1], message, l);	// envoie de l'opération entrée au clavier au fils
		close(Tube1[1]);
		l=read(Tube2[0], messageMaj, l);	// recepetion du résultat
		close(Tube2[0]); 
		messageMaj[l-1] = '\0'; //ajout du caractère de fin de chaine
		printf("Resultat:%s\n",messageMaj);
	}
    return 0;
}
