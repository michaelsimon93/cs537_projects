#include "types.h"
#include "user.h"
#include "stat.h"


int main(int argc, char* argv[]) {
	printf(1, "%x\n", 0xA0000 - 0x1000);
	char str[4100];
	int i;
	for (i = 0; i < 4098; i++) {
		str[i] = i%26 + 97;
	}
	str[4099] = '\0';
	printf(1,"%s\n",str);
	printf(1,"\tNow you know your abc's\n\tand have grown the stack.\n");
	exit();
}
