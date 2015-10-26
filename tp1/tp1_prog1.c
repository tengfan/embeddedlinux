#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

void main() {
	char c;
	int pid;
	pid = fork();
	if(pid == 0) {
		printf("pid fils = %d .\n", &pid);
		for(c='a';c<='z';c++){
			printf("%c ", c);
		}
	}
	else{
		 printf("pid pere: %d .\n",&pid);
		 for(c='A';c<='Z';c++){
			printf("%c ", c);
		}
	 }
	printf("Execute par les 2.\n");
}
