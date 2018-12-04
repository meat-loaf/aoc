#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUMCLAIMS 1375
#define DELIMS "#x:,\n "

struct point{
	unsigned short x;
	unsigned short y;
};

//top left, top right,
//bottom left, bottom right
struct claim {
	//short id;
	struct point tl;
	struct point tr;
	struct point bl;
	struct point br;
};
void init_claim(struct claim* claim, char *line){ // int id, int dleft, int dtop, int w, int h){
	int i = 0;
	int id, dleft, dtop, w, h;
	char *ptr;
	bool start = false;
	while ((ptr = strtok(start ? NULL : line, DELIMS)) != NULL){
		switch(i){
			case 0:
				id = atoi(ptr);
				break;
			case 4:
				dleft = atoi(ptr);
				break;
			case 5:
				dtop = atoi(ptr);
				break;
			case 7:
				w = atoi(ptr);
				break;
			case 8:
				h = atoi(ptr);
				break;
			default:
				break;
		}
		i++;
		start = true;
	}

	//claim->id = id;
	claim->tl.x = dleft; claim->tl.y = dtop; 
	claim->tr.x = dleft + w; claim->tr.y = dtop;
	claim->bl.x = dleft; claim->bl.x = dtop + h;
	claim->br.x = dleft + w; claim->br.y = dtop+h;
}

int main(int argc, char **argv){
	struct claim* claims = malloc(sizeof(struct claim)*NUMCLAIMS);
	if (claims == NULL){
		fprintf(stderr, "OUT OF MEMORY\n");
		exit(1);
	}
	int curr_line = 0;
	char *line = NULL;
	ssize_t n = 0;
	FILE *input_file;
	if (argc != 2){
		fprintf(stderr, "Please provide input file.\n");
		exit(1);
	}
	input_file = fopen(argv[1], "r");
	//while(getline(&line, &n, input_file) != -1){
	getline(&line, &n, input_file);
		init_claim(&(claims[curr_line]), line);
		curr_line++;
	//}
	//getline allocates for us
	free(line);
	//printf("solution: %d\n");
	return 0;
}
