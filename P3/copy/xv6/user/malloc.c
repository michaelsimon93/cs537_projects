#include "types.h"
#include "user.h"
#include "stat.h"
#define LEN 700

int main(int argc, char* argv[]) {
	
	char *alph = malloc(sizeof(char) * LEN);
	char *Alph = malloc(sizeof(char) * LEN);
	char *numb = malloc(sizeof(char) * LEN);
	char *symb = malloc(sizeof(char) * LEN);


	int i;

	for (i = 0; i < LEN-2; i++) alph[i] = i%26 + 97;
	for (i = 0; i < LEN-2; i++) Alph[i] = i%26 + 65;
	for (i = 0; i < LEN-2; i++) numb[i] = i%10 + 48;
	for (i = 0; i < LEN-2; i++) symb[i] = i%15 + 33;

	alph[LEN-1] = '\0';
	Alph[LEN-1] = '\0';
	numb[LEN-1] = '\0';
	symb[LEN-1] = '\0';

	printf(1, "%s\n%s\n%s\n%s\n", alph, Alph, numb, symb);
	printf(1, "sucessfully allocated several large character arrays\n");
	exit();
}
