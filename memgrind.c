#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int randomize(int size){
	time_t time;
	srand((unsigned) time(&time));
	return (rand() % size);
}

int main(int argc, char *argv){
	int workloadCount;
	gettimeofday(&tv, NULL);

	time_t workloadATime = 0;
	time_t workloadBTime = 0;
	time_t workloadCTime = 0;
	time_t workloadDTime = 0;
	time_t workloadETime = 0;
	time_t workloadFTime = 0;

	int i;
	int j = 0;
	int k = 0;
	char *p;

	struct timeval start, end;
	time_t totalTime = 0;

	// A: malloc() 1 byte and immediately free it - do this 150 times
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		for(i=0; i < 150; i++){	
			p = (char*)malloc(1);
			free(p);
		}
		gettimeofday(&end, NULL);
		workloadATime += (end - start);
	}


	// B: malloc() 1 byte, store the pointer in an array - do this 150 times.
 	// Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
	char *arr[150];
	int count = 0;
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		for(i=0; i < 150; i++){	
			arr[i] = (char*)malloc(1);
			if((i % 50) == 0){			//possible error in if and following for loop condition !!!!!
				for(j=j; count <= 50 ;j++){
					free(arr[j]);
					count++;
				}
				count = 0;
			}
		}
		gettimeofday(&end, NULL);
		workloadBTime += (end - start);
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
	char *arr[50];
	
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
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
		gettimeofday(&end, NULL);
		workloadCTime += (end - start);
		mallocCount = 0;
		freeCount = 0;
	}

	// D: Randomly choose between a randomly-sized malloc() or free()ing a pointer  do this many
 	// times (see below)
	// - Keep track of each malloc so that all mallocs do not exceed your total memory capacity
	// - Keep track of each operation so that you eventually malloc() 50 times
	// - Keep track of each operation so that you eventually free() all pointers
	// - Choose a random allocation size between 1 and 64 bytes
	mallocCount = 0;
	freeCount = 0;
	char *arr3[50];

	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		while(freeCount == 50 && mallocCount == 50){
			if(mallocCount == 50){
				free(arr3[freeCount]);
				freeCount++;
			}else{
				options = randomize(2);
				if(options == 0){	//malloc
					random = randomize(64);
					arr3[mallocCount] = (char*)malloc(random);
					mallocCount++;
				}else if(options == 1 && freeCount < mallocCount){	//free
					free(arr3[freeCount]);
					freeCount++;	
				}else{
					random = randomize(64);
					arr3[mallocCount] = (char*)malloc(random);
					mallocCount++;
				}
			}
		}
		gettimeofday(&end, NULL);
		workloadDTime += (end - start);
		mallocCount = 0;
		freeCount = 0;
	}
	//E:
	// Malloc() 25 bytes until reaching Full Capacity (we should know it is full if the pointer we malloced for is NULL)
	//Free() all pointers
	//malloc () 50 bytes until reaching full capaicity
	//free all pointers
	//From this we should be able to test if our combineBlocks()function works effectively

	mallocCount = 0;
	char *arr4[165];
	while(ptr == NULL){	//change header to what the output of NULL would be mallocs 25 bytes
		arr4[mallocCount] = (char*)malloc(25);
		mallocCount++;
	}
	i = 0;
	while(i <= mallocCount){	//free all pointers
		free(arr4[i]);
		i++;
	}
	mallocCount = 0;
	while(ptr == NULL){	//change header to what the output of NULL would be, mallocs 50 bytes
		arr4[mallocCount] = (char*)malloc(50);
		mallocCount++;
	}
	i = 0;
	while(i <= mallocCount){
		free(arr4[i]);
		i++;
	}
	//end of what i added
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		//E
		gettimeofday(&end, NULL);
		workloadETime += (end - start);
	}
	
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		//F
		gettimeofday(&end, NULL);
		workloadFTime += (end - start);
	}
}
