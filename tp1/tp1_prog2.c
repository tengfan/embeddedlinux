#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
void exit_func(int noSignal) {
	printf("Trop tard.\n");
	exit(-1);
}

void func(int noSignal){
	if(noSignal == SIGINT) {
		printf("Reveille par [CTRL]+[C], tps = %d \n", time(NULL));
	}
	else {
			alarm(1);
			printf("tps = %d\n", time(NULL));
	}
}

int main(void) {
	signal (SIGALRM, func);
	signal (SIGINT, func);
	signal (SIGALRM, exit_func);
	alarm(5);
	while(1);
	return 1;
}
