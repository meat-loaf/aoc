#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct twos_threes{
	unsigned int twos;
	unsigned int threes;
};

int compare(const void *l, const void *r){
	return (*(char *)l - *(char *)r);
}

void check_line(char *line, struct twos_threes* v){
	int i = 0;
	int sz = strlen(line);
	bool two = false;
	bool three = false;
	while(i < sz){
		if(i + 1 < sz && line[i] == line[i+1]){
			if (i+2 < sz && line[i] == line[i+2]){
				if (!three)
					v->threes++;
				i+=2;
				three = true;
			}
			else if (line[i] == line[i+1]){
				if (!two)
					v->twos++;
				i++;
				two = true;
			}
		}
		i++;
	if (two && three) return;
	}
}


int main(int argc, char **argv){
	struct twos_threes vals = {0};
	char *line = NULL;
	ssize_t n = 0;
	FILE *input_file;
	if (argc != 2){
		fprintf(stderr, "Please provide input file.\n");
		exit(1);
	}
	input_file = fopen(argv[1], "r");
	while(getline(&line, &n, input_file) != -1){
		int i;
		char *nloc;
		//trim the newline, if there is one
//		printf("%s", line);
		if ((nloc = strchr(line, '\n')) != NULL){
			*nloc = '\0';
		}
		qsort(line, strlen(line), sizeof(char), compare);
		check_line(line, &vals);
//		printf("%s\n\n", line);
	}
	//getline allocates for us
	free(line);
	printf("solution: %d\n", vals.twos * vals.threes);
	return 0;
}
