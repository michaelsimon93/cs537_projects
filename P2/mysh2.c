//mysh.c
//Benjamin Baeten
//Michael Simon

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_STR 128
#define MAX_HIS 20

int arg_c, 	 //number of arguments
redirect,	 //boolean if redirection is used
numCommands = 0; //number of total commands

char errmsg[30] = "An error has occurred\n";
int min(int x, int y) {
	if (x <= y) return x;
	else return y;
}
	

void updateHistory(char *inBufC, char **hisBuf) {

	numCommands++;
		//just in case there have been 1 million commands
	if (numCommands > 999999) numCommands = MAX_HIS;
	if (numCommands < MAX_HIS) {
		hisBuf[numCommands - 1] = strdup(inBufC);

	} else {
		int q = 1;
		while(q < MAX_HIS) {
			hisBuf[q-1] = hisBuf[q];
			q++;
		}
		hisBuf[MAX_HIS - 1] = strdup(inBufC);
	}	
	
	

}

int tokenize(char *input, char *inputC,  char **a_v, char* redir) {

	if ( strlen(input) >= 128 ) {
		write(STDERR_FILENO, errmsg, strlen(errmsg));
	}
	if (strlen(input) == 1) {
		return -1;
	}
	char *rPtr = strchr(input,'>');		//location of > symbol
	a_v = malloc( (arg_c + 1) * sizeof(char*));	//arg buffer
	
	//is redirection?
	if (rPtr != NULL) {
		*rPtr = '\0';
		redir = strdup(rPtr + 1);
		redirect = 1;
		fprintf(stdout, "%s\n", redir);
	}

	//count arguments
	char *newBuf = strdup(input);
	inputC = strdup(input);
	char *tPtr = strtok(newBuf, " \t");
	while(tPtr != NULL) {
		arg_c++;
		tPtr = (strtok(NULL, " \t"));
	}
	free(newBuf);
	
	//tokenize arguments
	int i = 0;
	tPtr = strtok(input, " \t\n");
	while (tPtr != NULL) {
		a_v[i] = strdup(tPtr);
		i++;
		tPtr = (strtok(NULL, " \t\n"));
			
	}
	return 1;


}

void execute(int a_c, char **a_v, const char *redir, const char **hisBuf) {

	//test for exit
	if (strcmp(a_v[0], "exit") == 0) exit(0);

	//list history
	if (strcmp(a_v[0], "history") == 0) {
		char *lineBuf = malloc( (MAX_STR + 7) * sizeof(char));
		int x = 0;
		int y;
		int lim = min(numCommands, MAX_HIS);
		while(x < lim) {
			if (numCommands > MAX_HIS) y = x 
			+ numCommands - MAX_HIS;
			else y = x;
			sprintf(lineBuf, "%d %s", y+1,
			hisBuf[x]);
			write(STDOUT_FILENO, lineBuf, strlen(lineBuf));
			x++;
		}
		free(lineBuf);
	//running child program
	} else { 
		int rc = fork();
		if (rc == 0) {
			int fn = dup(STDOUT_FILENO);
			if (redirect && !close(STDOUT_FILENO)) {
				if (open(redir, O_CREAT | O_WRONLY |
				O_TRUNC, S_IRWXU) == -1) {
					dup2(fn, STDOUT_FILENO);
				
				}

			}
			execvp(a_v[0], a_v);
			exit(0);
		}


	}
	updateHistory(inBufC, hisBuf);

}

int main(int argc, char **argv) {

	//buffers for input string and cuommand history
	char inBuf[MAX_STR+1];
	char **arg_v;	
	char **hisBuf = malloc(MAX_HIS * sizeof(char*));
	char *redir = NULL, //redirect string
	*inBufC = strdup(input);	//copy of command for history

	int x = 0;
	while (x < MAX_HIS) {
		hisBuf[x] = malloc((MAX_STR) * sizeof(char));
		x++;
	}



	while(1) {
		arg_c = 0;
		redirect = 0;
		fgets(inBuf,MAX_STR+1,stdin);
		if ( tokenize(inBuf, inBufC, arg_v, redir) == -1) continue;
		execute(arg_c, arg_v, redir, hisBuf);
		/*
		 * fork and exec issued command
		 * figure out redirection
		 */
		free(inBufC);
		free(arg_v);
	}


return 0;
}
