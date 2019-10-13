#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int randomize(int size){
	time_t time;
	srand((unsigned) time(&time));
	return (rand() % size);
}

int main(int argc, char *argv){
	int i;
	int j = 0;
	int k = 0;
	char *p;
	// A: malloc() 1 byte and immediately free it - do this 150 times
	for(i=0; i <= 150; i++){	
		p = (char*)malloc(1);
		free(p);
	}

	// B: malloc() 1 byte, store the pointer in an array - do this 150 times.
 	// Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
	char *arr[150];
	int count = 0;
	for(i=0; i <= 150; i++){		// <= condition may be 1 too many
		arr[i] = (char*)malloc(1);
		if((i % 50) == 0){
			for(j=j; count <= 50 ;j++){
				free(arr[j]);
				count++;
			}
			count = 0;
		}
	}
	
	//C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
	//	> do this until you have allocated 50 times
	//- Keep track of each operation so that you eventually malloc() 50 bytes, in total
	//	> if you have already allocated 50 times, disregard the random and just free() on each
	//	iteration
	//- Keep track of each operation so that you eventually free() all pointers
	//	> don't allow a free() if you have no pointers to free()
	int mallocCount = 0;
	int freeCount = 0;
	int random;
	char *arr2[50];
	while(freeCount == 50 && mallocCount == 50){
		if(mallocCount == 50){
			free(arr2[freeCount]);
			freeCount++;
		}else{
			random = randomize(2);
			if(random == 0){	//malloc
				arr2[mallocCount] = (char*)malloc(1);
				mallocCount++;
			}else if(random == 1 && freeCount < mallocCount){	//free
				free(arr2[freeCount]);
				freeCount++;
			}else{		//malloc, because there isn't any current pointers to free
				arr2[mallocCount] = (char*)malloc(1);
				mallocCount++;
			}
		}
	}

	// D: Randomly choose between a randomly-sized malloc() or free()ing a pointer  do this many
 	// times (see below)
	// - Keep track of each malloc so that all mallocs do not exceed your total memory capacity
	// - Keep track of each operation so that you eventually malloc() 50 times
	// - Keep track of each operation so that you eventually free() all pointers
	// - Choose a random allocation size between 1 and 64 bytes
	mallocCount = 0;
	freeCount = 0;
	int options;
	count = 4096;
	char *arr3[50];
	while(freeCount == 50 && mallocCount == 50){
		if(mallocCount == 50){
			free(arr3[freeCount]);
			freeCount++;
		}else{
			options = randomize(2);
		}
		if(options == 0){	//malloc		
			arr3[mallocCount] = (char*)malloc(random);
			mallocCount++;		
		}else if(options == 1 && freeCount < mallocCount){	//free
			free(arr3[freeCount]);
			freeCount++;		
		}else{
			arr3[mallocCount] = (char*)malloc(random);
			mallocCount++;
		}
		
	}


}
