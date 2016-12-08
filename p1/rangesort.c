#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>


void usage(char *prog) {
    fprintf(stderr, "usage: %s <-s random seed> <-n number of records> <-o output file>\n", prog);
    exit(1);
}

//function to compare the inputs 
int cmpfunc(const void *a, const void *b) {
	
	return ( *(int*)a - *(int*)b );
	
}

int main(int argc, char *argv[]) {
	
	assert(sizeof(rec_t) == 100);

	if (argc != 9) {
		usage(argv[0]);
	}
	
	//variables
	char *outFile   = "/no/such/file";
	char *inFile = "/no/such/file";
	char *p;
	char *p2;
	char lowCheck[11];
	char highCheck[11];
	int i;
	int c = 0;
	unsigned int low;
	unsigned int high;
	
	//check the command line 
    while ((c = getopt(argc, argv, "i:o:l:h:")) != -1) {
		switch (c) {
			
			case 'i':
				inFile = strdup(optarg);
				break;
		
			case 'o':
				outFile = strdup(optarg);
				break;
		
			case 'l':
				p = strdup(optarg);
				unsigned long int longLow = strtol(p, NULL, 0);
				i = 0;
				
				if (longLow > 4294967295) {
					low = 4294967295;
				} else {
					low = longLow;
				}
				
				while (i < 10) {
					if (*p == '\0') {
						lowCheck[i] = '\0';
						break;
					}
					
					if (*p < 48 || *p > 57) {
						fprintf(stderr, "Error: Invalid range value");
						exit(1);
					}
					
					p++;
					i++;
				}
				
				break;
	
			case 'h':
				p2 = strdup(optarg);
				unsigned long int longHigh = strtol(p2, NULL, 0);
				i = 0;
				if (longHigh > 4294967295) {
					high = 4294967295;
				} else {
					high = longHigh;
				}
				
				while (i < 10) {
					if (*p2 == '\0') {
						highCheck[i] = '\0';
						break;
					}
					
					if (*p2 < 48 || *p2 > 57) {
						fprintf(stderr, "Error: Invalid range value");
						exit(1);
					}
					
					p2++;
					i++;
				}
				break;
			default:
				usage(argv[0]);
		}
    }
    
	if (low > high) {
		fprintf(stderr, "Error: Invalid range value\n");
		exit(1);
	}
    
    // open the input file (from dump)
    int inputfd = open(inFile, O_RDONLY);
    if (inputfd < 0) {
		perror("open");
		exit(1);
    }
    
    //open the output file
    int outputfd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (inputfd < 0) {
		perror("open");
		exit(1);
    }
    
    rec_t r;
    int numRecInRange = 0;
    
    //read the input file and then 
    //loop to figure out how many records fall within the range 
    while (1) {	
		int rc;
		rc = read(inputfd, &r, sizeof(rec_t));
		if (rc == 0) {
			break;
		}
		if (rc < 0) {
			perror("read");
			exit(1);
		}
		if (r.key >= low && r.key <= high) {
			numRecInRange++;
		}
    }
    
    (void) close(inputfd);

	inputfd = open(inFile, O_RDONLY);
    if (inputfd < 0) {
		perror("open");
		exit(1);
    }
	
	rec_t *size = malloc(sizeof(rec_t)*numRecInRange);
	rec_t *recsInRange = &size[0];
	
	//loop through to add the numbers in the range to 
	//an array
	while (1) {	
		int rc;
		rc = read(inputfd, &r, sizeof(rec_t));
		if (rc == 0) { // 0 indicates EOF
			break;
		}
		if (rc < 0) {
			perror("read");
			exit(1);
		}
		if (r.key >= low && r.key <= high) {
			*recsInRange = r;
			recsInRange++;
		}
    }
    
    (void) close(inputfd);
    
	//call qsort and create and write the output file (generate)
	qsort(size, numRecInRange, sizeof(rec_t), cmpfunc);
	
    i = 0;
    while (i < numRecInRange) {
		r = size[i];
		int rc = write(outputfd, &r, sizeof(rec_t));
		if (rc < 0) {
			fprintf(stderr, "Error: Invalid range value \n");
			exit(1);
		}
		i++;
	}
    
    (void) close(outputfd);
	
	return 0;
}
