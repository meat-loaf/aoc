#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#define FILELINE 17
#define FILELINE 934
#define DELIMS "[-:] #"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

struct elf_schedule{
	//note: id may be 0. in an array of schedules,
	//this means the id is the last-seen non-0 id.
	unsigned short id;
	unsigned short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
//	bool begin;
	bool asleep;
};

struct guard {
	unsigned short id;
	unsigned int minutes[60];
	struct guard *next;
};

int compare_scheds(const void *first, const void *second){
	struct elf_schedule *f = (struct elf_schedule *)first;
	struct elf_schedule *s = (struct elf_schedule *)second;
	int val;
	val = f->year - s->year;
	if (val) return val;
	val = f->month - s->month;
	if (val) return val;
	val = f->day - s->day;
	if (val) return val;
	val = f->hour - s->hour;
	if (val) return val;
	val = f->minute - s->minute;
	if (val) return val;
	val = f->id - s->id;
	if (val) return val;
	return f->asleep;
}

void init_schedule_slot(char *line, struct elf_schedule* sched){
	int i = 0;
	bool donullparam = false;
	char *secondhalf = NULL, *ptr = NULL;
	while((ptr = strtok(!donullparam ? line : NULL, DELIMS))){
		switch(i){
		case 0:
			donullparam = true;
			sched->year = atoi(ptr);
			break;
		case 1:
			sched->month = atoi(ptr);
			break;
		case 2:
			sched->day = atoi(ptr); 
			break;
		case 3:
			sched->hour = atoi(ptr);
			break;
		case 4:
			sched->minute = atoi(ptr);
			break;
		case 5:
			//at this point the data begins...
			//Guard #x begins shift
			//'falls asleep'
			//'wakes up'
			if (!strcmp(ptr, "wakes")){
//				sched->begin = false;
				sched->asleep = false;	
				sched->id = 0;
			}
			else if (!strcmp(ptr, "falls")){
				sched->asleep = true;
				sched->id = 0;
			}
			else if (!strcmp(ptr, "Guard")){
				//get the guards number by calling strcmp once more.
				sched->asleep = false;
				ptr = strtok(NULL, DELIMS);
				sched->id = atoi(ptr);
			}
			//at this point we have all the data we need,
			//don't bother to strtok the rest of the string
			//goto done;
			return;
		}
		i++;
	}
}

struct guard *new_guard(int id){
	struct guard *g = malloc(sizeof(struct guard));
	g->id = id;
	for(int i = 0; i < 60; i++){
		g->minutes[i] = 0;
	}
	g->next = NULL;
	return g;
}

struct guard *guard_list_insert(struct guard *gs, int id){
	while(gs){
		if (gs->next == NULL){
			gs->next = new_guard(id);
			return gs->next;
		}
		gs = gs->next;
	}
	return NULL;
}

struct guard *guard_list_contains(struct guard *gs, int id){
	while(gs){
		if (gs->id == id){
			return gs;
		}
		gs = gs->next;
	}
	return NULL;
}

void update_range(struct guard* curr, unsigned char start, unsigned char end){
	while (start != end){
		((curr->minutes)[start])++;
		start++;
	}
}

int get_checksum(struct guard* guards){
	struct guard *sleepiest = guards;
	int minutes = 0, i, sleepiest_min = 0;
	/*for (i = 0; i < 60; i++){
		minutes += sleepiest->minutes[i];
	}*/
	while(guards){	
		int minutes_chk = 0;
		for (i = 0; i < 60; i++){
			if (guards->minutes[i] > minutes){
				sleepiest = guards;
				minutes = guards->minutes[i];
				sleepiest_min = i;
			}
		}
		//count how long the guard was asleep
		/*for (i = 0; i < 60; i++){
			minutes_chk += guards->minutes[i];
		}*/
		//if the new guard had more minutes asleep,
		//replace the sleepiest guard
		/*if (minutes_chk > minutes){
			sleepiest = guards;
			minutes = minutes_chk;
		}*/
		guards = guards->next;
	}
	minutes = 0;

	return sleepiest->id * sleepiest_min;
}


int find_best_elf(struct elf_schedule* scheds, int schedsize){
	int i, gsize = 0;
	//minute the guard starts sleeping
	//60 is invalid, use to check error
	unsigned char startrange = 60;
	struct guard *guards = NULL;
	struct guard *gfirst = NULL;
	struct guard* curr;
	if (guards == NULL && scheds[i].id != 0){
		gfirst = guards = new_guard(scheds[i].id);
	}
	for (i = 0; i < schedsize; i++){
		if (scheds[i].id){
			curr = guard_list_contains(guards, scheds[i].id);
		
			if (curr == NULL){
				curr = guard_list_insert(guards, scheds[i].id);
			}
		}
		//started sleeping
		else if (scheds[i].asleep == true){
			startrange = scheds[i].minute;
		}
		else if (scheds[i].asleep == false){
			if (startrange == 60 || startrange > scheds[i].minute){
				fprintf(stderr, "data invalid, found wake event before sleep event\n");
				exit(1);
			}
			update_range(curr, startrange, scheds[i].minute);
			startrange = 60;
		}
	}
	return get_checksum(gfirst);
}

int main(int argc, char **argv){
	int curr_line = 0;
	int size = FILELINE;
	char *line = NULL;
	ssize_t n = 0;
	FILE *input_file;
	struct elf_schedule *sched = malloc(sizeof(struct elf_schedule) *FILELINE);
	if (!sched){
		fprintf(stderr, "OUT OF MEMORY\n");
		exit(1);
	}
	if (argc != 2){
		fprintf(stderr, "Please provide input file.\n");
		exit(1);
	}
	input_file = fopen(argv[1], "r");
	while(getline(&line, &n, input_file) != -1){
		init_schedule_slot(line, &(sched[curr_line]));
		curr_line++;
		if (curr_line>size){
			size *= 2;
			sched = realloc(sched, sizeof(struct elf_schedule)*size);
			if (!sched){
				fprintf(stderr, "OUT OF MEMORY\n");
				exit(1);
			}
		}
	}
	qsort(sched, curr_line, sizeof(struct elf_schedule), compare_scheds);
	printf("solution: %d\n", find_best_elf(sched, curr_line));
	free(sched);
	//getline allocates for us
	free(line);
	return 0;
}
