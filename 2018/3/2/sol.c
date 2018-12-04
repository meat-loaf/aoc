#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUMCLAIMS 1375
#define DELIMS "@#x:,\n "
#define GRIDROWS 1000
#define GRIDCOLS 1000
 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


struct point{
	unsigned short x;
	unsigned short y;
};

//top left, top right,
//bottom left, bottom right
struct claim {
	short id;
	struct point tl;
	struct point br;
	bool intersect;
};
void init_claim(struct claim* claim, char *line){ // int id, int dleft, int dtop, int w, int h){
	int i = 0;
	int id, dleft, dtop, w, h;
	char *ptr;
	ptr = strtok(line, DELIMS);
	id = atoi(ptr);
	i++;
	while ((ptr = strtok(NULL, DELIMS)) != NULL){
		switch(i){
			case 0:
				id = atoi(ptr);
				break;
			case 1:
				dleft = atoi(ptr);
				break;
			case 2:
				dtop = atoi(ptr);
				break;
			case 3:
				w = atoi(ptr);
				break;
			case 4:
				h = atoi(ptr);
				break;
			default:
				break;
		}
		i++;
	}

	claim->id = id;
	claim->tl.x = dleft; claim->tl.y = dtop; 
	claim->br.x = dleft + w; claim->br.y = dtop+h;
	claim->intersect = false;
}

void mark_intersect(struct claim *a, struct claim *b){
	//if the intersection exists, it will be a rectangle.
	struct claim intersect;
	int w, h, i, j;
	intersect.tl.x = max(a->tl.x, b->tl.x);
	intersect.tl.y = max(a->tl.y, b->tl.y);
	intersect.br.x = min(a->br.x, b->br.x);
	intersect.br.y = min(a->br.y, b->br.y);
	//check for degeneracy (if exists areas dont intersect)
	if(intersect.tl.x > intersect.br.x ||
		intersect.tl.y > intersect.br.y){
		return;
	}
	a->intersect = b->intersect = true;
	return;
}

int find_nonintersect(struct claim *claims){
	int i, j;
	bool found = false;
	for (i = 0; i < NUMCLAIMS; i++){
		for (j = 0; j < NUMCLAIMS; j++){
			if (i == j) break;
			mark_intersect(&claims[i], &claims[j]);
		}
	}
	for (i = 0; i < NUMCLAIMS; i++){
		if (!claims[i].intersect)
		return claims[i].id;
	}
	return 0;
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
	while(getline(&line, &n, input_file) != -1){
		init_claim(&(claims[curr_line]), line);
		curr_line++;
	}	
	//getline allocates for us
	free(line);
	printf("solution: %d\n", find_nonintersect(claims));
	free(claims);
	return 0;
}
