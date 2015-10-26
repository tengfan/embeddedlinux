#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int *p = NULL;
void majHandler(int num){
	p = (int *)malloc(sizeof(int));
	printf("Dans le handler.\n");
}

int main() {
	signal(SIGSEGV,majHandler);
	*p = 12;
	printf("%d\n",p);
	return 1;
}
