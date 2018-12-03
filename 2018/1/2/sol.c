#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FPATH "./in.txt"

//LONG_MIN is –2147483648;
//11 chars + 2 for newline, null term
#define LINESZ 12
#define OOM "OUT OF MEMORY\n"
#define LISTLENINIT 1024

/*static long *freqlist = NULL;
static long listlen = 0;
static int listrange = LISTLENINIT;*/
static struct llwrap *list = NULL;

struct ll {
	long v;
	struct ll* next;
};

struct llwrap {
	long sz;
	struct ll* first;
	struct ll* last;
};



bool list_contains(long val){
	if (list == NULL) return false;
	struct ll* tmp = list->first;
	while(tmp != NULL){
		if (tmp->v == val)
			return true;
		tmp = tmp->next;
	}
	return false;
}

void update_list(long val){
	if (list == NULL){
		list = malloc(sizeof(struct llwrap));
		list->sz == 0;
		list->last = malloc(sizeof(struct ll));
		list->first = list->last;
		list->last->next = NULL;
	}
	list->last->v = val;
	list->sz++;
	list->last->next = malloc(sizeof(struct ll));
	list->last = list->last->next;
	list->last->next = NULL;
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
