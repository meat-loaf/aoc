#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FPATH "./in.txt"

//LONG_MIN is –2147483648;
//11 chars + 2 for newline, null term
#define LINESZ 12
#define OOM "OUT OF MEMORY\n"
#define LISTLENINIT 1024

static long *freqlist = NULL;
static long listlen = 0;
static int listrange = LISTLENINIT;

bool list_contains(long val){
	long i = 0;
	for (i = 0; i < listlen; i++){
		if (freqlist[i] == val)
			return true;
	}
	return false;
}

void update_list(long val){
	if (freqlist == NULL && listlen == 0){
		freqlist = malloc(sizeof(long)*LISTLENINIT);
		if (freqlist == NULL){
			fprintf(stderr, OOM);
			exit(1);
		}
	}
	freqlist[listlen] = val;
	listlen++;
	if(listlen >= listrange){
		void *tmp = realloc(freqlist, listrange * 2);
		if (tmp == NULL){
			fprintf(stderr, OOM);
			free(freqlist);
			exit(1);
		}
		freqlist = tmp;
		listrange = listrange * 2;
	} 
}		

int main(int argc, char **argv){
	signed long sum = 0;
	ssize_t n = LINESZ;
	char *line = malloc(LINESZ);
	if (line == NULL){
		fprintf(stderr, OOM);
		exit(1);
	}
	FILE *f = fopen(FPATH, "r");
	if (f == NULL){
		printf("couldn't open file %s\n", FPATH);
		free(line);
		exit(2);
	}
top:
	while(getline(&line, &n, f) != -1){
		sum += strtol(line, NULL, 10);
		if (!list_contains(sum))
			update_list(sum);
		else {
			printf("solution: %ld\n", sum);
			return 0;
		}
	}
	rewind(f);
	goto top;
}
