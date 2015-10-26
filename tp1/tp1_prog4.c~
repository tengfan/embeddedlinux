#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void explosePile() {
	int a;
	printf("%x\n", &a);
	explosePile();
}

// ./a.out > fichier.txt
// tail fichier.txt
// head fichier.txt
//Taille de pile = 8330304 dans le pc de l'ensi
int main(void) {
	explosePile();
	return 1;
}
