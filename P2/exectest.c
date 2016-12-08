#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char **argv) {
int rc = fork();

	if (rc == 0) {
		execvp("/bin/ls","-l");
	} else {
		fprintf(stdout, "done\n");
	}



return 0;
}
