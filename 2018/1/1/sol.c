#include <stdlib.h>
#include <stdio.h>

#define FPATH "./in.txt"

//LONG_MIN is –2147483648;
//11 chars + 2 for newline, null term
#define LINESZ 12

int main(int argc, char **argv){
	signed long sum = 0;
	ssize_t n = LINESZ;
	char *line = malloc(LINESZ);
	if (line == NULL){
		printf("out of memory\n");
		exit(1);
	}
	FILE *f = fopen(FPATH, "r");
	if (f == NULL){
		printf("couldn't open file %s\n", FPATH);
		free(line);
		exit(2);
	}
	while(getline(&line, &n, f) != -1)
		sum += strtol(line, NULL, 10); 
	printf("sum: %ld\n", sum);
	
	return 0;	
}
