#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define FILELEN 250
#define LINELEN 27

struct soln{
	int first;
	int second;
};

bool strcmp_oneoff(char *l, char *r){
	bool mark = false;
	while(*l && *r){
		if (*l != *r)
			if (mark) return false;
			else mark = true;
		l++;
		r++;
	}
	return true;
}

void check(char data[FILELEN][LINELEN], struct soln* s){
	int i, j;
	for (i = 0; i < FILELEN; i++){
		for (j = 0; j < FILELEN; j++){
			if (i == j) break;
			if(strcmp_oneoff(data[i], data[j])){
				s->first = i;
				s->second = j;
			}
				
		}
	}
}

int main(int argc, char **argv){
	struct soln s = {0};
	char *line = NULL;
	char data[FILELEN][LINELEN];
	ssize_t n = 0;
	FILE *input_file;
	if (argc != 2){
		fprintf(stderr, "Please provide input file.\n");
		exit(1);
	}
	input_file = fopen(argv[1], "r");
	int lnum = 0;
	while(getline(&line, &n, input_file) != -1){
		int i;
		char *nloc;
		//trim the newline, if there is one
		if ((nloc = strchr(line, '\n')) != NULL){
			*nloc = '\0';
		}
		strncpy(data[lnum], line, 27);
		lnum++;
	}
	check(data, &s);
	printf("%s\n%s\n", data[s.first], data[s.second]);
	//getline allocates for us
	free(line);
	return 0;
}
