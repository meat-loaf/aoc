#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//-1 lower bound/+1 upper bound to make
//range inclusive
#define ASCII_CAPS_START	('A'-1)
#define ASCII_CAPS_END		('Z'+1)
#define ASCII_NCAPS_START	('a'-1)
#define ASCII_NCAPS_END		('z'+1)

#define ALPHA_DIFF ('a'-'A')

#define DELETED_CHAR		'_'

int alpha_strlen(char *s){
	int count = 0;
	while(*s){
		char v = *s;
		if ((v - ASCII_CAPS_START > 0 &&
			ASCII_CAPS_END - v > 0)||
			(v - ASCII_NCAPS_START > 0  &&
			ASCII_NCAPS_END - v > 0)){
		count ++;
		}
	s++;
	}
	return count;
}

int find_next_type(char *line, int i){
	while(*(line+i)){
		if (*(line+i) != DELETED_CHAR){
			return i;
		}
		i++;
	}
	return -1;
}

int find_prev_type(char *line, int i){
	while(i > 0){
		if (*(line+i) != DELETED_CHAR){
			return i;
		}
		i--;
	}
	return -1;
}

void react_polymers(char *line){
	int i, next = -1, prev = -1, len = strlen(line);
	for (i = 0; i < len; i++){
		if (line[i] == DELETED_CHAR) continue;
		next = find_next_type(line, i+1);
		prev = find_prev_type(line, i-1);
		if (prev != -1){
			//the difference between capital/lowercase letters that are the same doesnt change.
			//all we need to to is check for the value
			if (!(abs(line[prev]-line[i]) - ALPHA_DIFF)){
				line[prev] = '_';
				line[i] = '_';
				i = find_prev_type(line, prev);
				continue;
			}
		}
		if (next != -1){
			if (!(abs(line[next]-line[i]) - ALPHA_DIFF)){
				line[next] = '_';
				line[i] = '_';
				continue;	
			}
		}
		else return;
	}	
}



int main(int argc, char **argv){
	char *line = NULL;
	ssize_t n = 0;
	FILE *input_file;
	if (argc != 2){
		fprintf(stderr, "Please provide input file.\n");
		exit(1);
	}
	input_file = fopen(argv[1], "r");
	getline(&line, &n, input_file);
	fclose(input_file);
	react_polymers(line);
//	printf("result:\n%s\n", line);
	printf("solution: %d\n", alpha_strlen(line));
	//getline allocates for us
	free(line);
	return 0;
}
