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
	//struct point tr;
	//struct point bl;
	struct point br;
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
	//claim->tr.x = dleft + w; claim->tr.y = dtop;
	//claim->bl.x = dleft; claim->bl.y = dtop + h;
	claim->br.x = dleft + w; claim->br.y = dtop+h;
}

void check_intersect(struct claim a, struct claim b, char grid[GRIDROWS][GRIDCOLS]){
	//if the intersection exists, it will be a rectangle.
	struct claim intersect;
	int w, h, i, j;
	intersect.tl.x = max(a.tl.x, b.tl.x);
	intersect.tl.y = max(a.tl.y, b.tl.y);
	intersect.br.x = min(a.br.x, b.br.x);
	intersect.br.y = min(a.br.y, b.br.y);
	//check for degeneracy (if exists areas dont intersect)
	if (intersect.tl.x > intersect.br.x ||
		intersect.tl.y > intersect.br.y){
		return;
	}
	w = intersect.br.x - intersect.tl.x;
	h = intersect.br.y - intersect.tl.y;
	for (i = 0; i < w; i++){
		for (j = 0; j < h; j++){
			grid[intersect.tl.y+j][intersect.br.x-i] = 'x';
		}
	}

}

int find_intersects(struct claim *claims){
	int i, j, soln = 0;
	char grid[GRIDROWS][GRIDCOLS] = {0};
	for (i = 0; i < NUMCLAIMS; i++){
		for (j = 0; j < NUMCLAIMS; j++){
			if (i == j) break;
			check_intersect(claims[i], claims[j], grid);
		}
	}
	for (i = 0; i < GRIDROWS; i++){
		for (j = 0; j < GRIDCOLS; j++){
			if (grid[i][j] == 'x')
				soln++;
		}
	}
	return soln;
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
	printf("solution: %d\n", find_intersects(claims));
	free(claims);
	return 0;
}
