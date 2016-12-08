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
#include <sys/wait.h>

#define MAX_STR 512
#define MAX_HIS 20



int min(int x, int y) {
	if (x <= y) return x;
	else return y;
}
	

int main(int argc, char **argv) {
	FILE *fstream;
	char errmsg[30] = "An error has occurred\n";
	if (argc == 2) {
		fstream = fopen(argv[1], "r");
		if (fstream == NULL) {
			write(STDERR_FILENO, errmsg, strlen(errmsg));
			exit(1);
		}
	} else if (argc != 1) {
		write(STDERR_FILENO, errmsg, strlen(errmsg));
		exit(1);
	}

	//buffers for input string and cuommand history
	char inBuf[MAX_STR+1];
	char **hisBuf = malloc(MAX_HIS * sizeof(char*));

	int x = 0;
	while (x < MAX_HIS) {
		hisBuf[x] = malloc((MAX_STR) * sizeof(char));
		x++;
	}

	int 
	redirect,	 //boolean if redirection is used
	numCommands = 0, //number of total commands
	hisCom = 0,
	arg_c;		 //number of arguments
	size_t len = 0;
	char *line = NULL;
	char prompt[8] = "mysh # ";
	while(1) {
		arg_c = 0;
		redirect = 0;
		if (!hisCom) {
			if (argc == 1) {
				write(STDOUT_FILENO, prompt, strlen(prompt));
			}
			if (argc == 1) fgets(inBuf,MAX_STR+1,stdin);
			else if (getline(&line,&len,fstream) == -1) exit(0);
			else {
				size_t _len = strlen(line);
				if (_len > MAX_STR) {
					_len = MAX_STR;
					write(STDOUT_FILENO, line, _len);
					write(STDOUT_FILENO, "\n", 1);
				} else write(STDOUT_FILENO, line, strlen(line));

				if (strlen(line) > MAX_STR+1) {
					write(STDERR_FILENO, errmsg, strlen(errmsg));
					continue;
				}
				sprintf(inBuf,"%s",line);
			}

		}
		if (strlen(inBuf) == 1) {
			continue;
		}
		char
		*redir = NULL, //redirect string
		*rPtr = strchr(inBuf,'>'),	//location of > symbol
		*inBufC = strdup(inBuf);	//copy of command for history
		char **arg_v = malloc((arg_c + 1) * sizeof(char*));	//arg buffer
		
		//is redirection?
		if (rPtr != NULL) {
			*rPtr = '\0';
			redir = strtok(rPtr + 1, " \t\n");
			int i = 0;
			while (strtok(NULL, " \t\n") != NULL) {
				i++;
			}
			if (i != 0) write(STDERR_FILENO, errmsg, strlen(errmsg));
			redirect = 1;
		}

		//count arguments
		char *newBuf = strdup(inBuf);
		char *tPtr = strtok(newBuf, " \t");
		while(tPtr != NULL) {
			arg_c++;
			tPtr = (strtok(NULL, " \t"));
		}
		free(newBuf);
		
//		fprintf(stdout,"%d\n",arg_c);
		//tokenize arguments
		int i = 0;
		tPtr = strtok(inBuf, " \t\n");
		while (tPtr != NULL) {
			arg_v[i] = strdup(tPtr);
			i++;
			tPtr = (strtok(NULL, " \t\n"));
				
		}
		arg_v[i+1] = NULL;
		if (!hisCom && arg_v[0][0] != '!') {
			numCommands++;
			//
				//just in case there have been 1 million commands
			if (numCommands > 999999) numCommands = MAX_HIS;
			if (numCommands <= MAX_HIS) {
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
		hisCom = 0;
		//test for exit
		if (strcmp(arg_v[0], "exit") == 0) {
			if (arg_c != 1){
				write(STDERR_FILENO, errmsg, strlen(errmsg));
				continue;
			}
			else exit(0);
		} 

		//list history
		if (strcmp(arg_v[0], "history") == 0) {
			if (arg_c > 1) {
				write(STDERR_FILENO, errmsg, strlen(errmsg));
			} else {	
				char *lineBuf = malloc( (MAX_STR + 7) * sizeof(char));
				int x = 0;
				int y;
				int d;

				int lim = min(numCommands, MAX_HIS);
				x = 0;
				while(x < lim) {
					if (numCommands >= MAX_HIS) {
						y = MAX_HIS - numCommands + x;
						d = x + 1 + numCommands - MAX_HIS;
					} else {
						y = x;
						d = x+1;
					}
					sprintf(lineBuf, "%d %s", d,
					hisBuf[x]);
					write(STDOUT_FILENO, lineBuf, strlen(lineBuf));
					x++;
				}
				free(lineBuf);
			}
		} else if(arg_v[0][0] == '!') {
			if (arg_c != 1) write(STDERR_FILENO, errmsg, strlen(errmsg));
			else {
				int n;
				if (strlen(arg_v[0]) == 1) n = 1;
				else n = (int)strtol(&arg_v[0][1], NULL, 10);
				int y;
				if (numCommands > MAX_HIS) y =1 +  MAX_HIS - numCommands + n;
				else y = n;
				if (y > 0 && y <= numCommands && y <= MAX_HIS) {
					sprintf(inBuf, "%s", hisBuf[y-1]);
					sprintf(inBufC, "%s", hisBuf[y-1]);
					hisCom = 1;
				} else {
					write(STDERR_FILENO, errmsg, strlen(errmsg));
				//	hisCom = 0;
				}
			}
		} else {
			int rc = fork();
			if (rc == 0) {
				int fn = dup(STDOUT_FILENO);
				if (redirect && !close(STDOUT_FILENO)) {

					
					if (open(redir, O_CREAT | O_WRONLY |
					O_TRUNC, S_IRWXU) == -1) {
						write(STDERR_FILENO, errmsg, strlen(errmsg));
						exit(1);
						dup2(fn, STDOUT_FILENO);
					
					}

				}
				execvp(arg_v[0], arg_v);
				if (!redirect) write(STDERR_FILENO, errmsg, strlen(errmsg));
				exit(1);
				
			} else (void) wait(NULL);


		}

		
		
		
	//	free(arg_v);
		/*
		 * fork and exec issued command
		 * figure out redirection
		 */
	}


return 0;
}
