#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(void)
{
	pid_t pid_fils;
	int tube1[2];
	int tube2[2];
	int i, l;
	
	unsigned char father_mes[50];
	unsigned char child_mes[50];

	pipe(tube1);
	pipe(tube2);

	pid_fils = fork();
	while (1){
		if(pid_fils == 0){
			dup2(tube1[0],0);
			dup2(tube2[1],1);
			close(tube1[1]);
			close(tube2[0]);  
			execl("/usr/bin/bc","bc",NULL);
			close(tube1[0]);
			close(tube2[1]);
		}
		else{
			close(tube1[0]);
			close(tube2[1]); 
			printf("Enter an operation\n");
			fgets(father_mes, 50, stdin);
			l=strlen(father_mes);
			write(tube1[1], father_mes, l);
			l=read(tube2[0], child_mes, l);
			child_mes[l-1] = '\0';
			printf("= %s\n",child_mes);
			printf("%s\n",father_mes);
		}
	}
	close(tube1[1]);
	close(tube2[0]);
    return 0;
}
