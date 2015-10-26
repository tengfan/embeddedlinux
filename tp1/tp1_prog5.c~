#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

int main() {
	struct rlimit rl;
	//Demander la taille de la pile
	getrlimit(RLIMIT_STACK, &rl);
	printf("Taille de la pile : %d\n", (int) rl.rlim_cur);
	
	//Modifier la taille de la pile
	rl.rlim_cur=rl.rlim_cur*6;
	setrlimit(RLIMIT_STACK, &rl);
	getrlimit(RLIMIT_STACK, &rl);
	printf("Taille de la pile : %d\n", (int) rl.rlim_cur);
	return 1;
}
