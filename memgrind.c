#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"mymalloc.h"
/*
int randomize(int size){
	time_t time;
	srand((unsigned) time(&time));
	return (rand() % size);
}	*/

int main(int argc, char *argv){
/*	int workloadCount;

	double workloadATime = 0;
	double workloadBTime = 0;
	double workloadCTime = 0;
	double workloadDTime = 0;
	double workloadETime = 0;
	double workloadFTime = 0;

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
		workloadATime += difftime(end - start);
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
		workloadBTime += difftime(end - start);
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
		workloadCTime += difftime(end - start);
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
		workloadDTime += difftime(end - start);
		mallocCount = 0;
		freeCount = 0;
	}

	//E:
	// Malloc() 25 bytes until reaching Full Capacity (we should know it is full if the pointer we malloced for is NULL)
	//Free() all pointers
	//malloc () 50 bytes until reaching full capaicity
	//free all pointers
	//From this we should be able to test if our combineBlocks()function works effectively
	char *arr4[165];
	char *ptr;
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		for(ptr = (char*)malloc(25); ptr != NULL; ptr = (char*)malloc(25)){
			arr4[mallocCount] = ptr;
			mallocCount++;
		}
		while(freeCount < mallocCount){	//free all pointers
			free(arr4[freeCount]);
			freeCount++;
		}
		mallocCount = 0;
		freeCount = 0;
		for(ptr = (char*)malloc(50); ptr != NULL; ptr = (char*)malloc(50)){
			arr4[mallocCount] = (char*)malloc(50);
			mallocCount++;
		}
		while(freeCount < mallocCount){
			free(arr4[freeCount]);
			freeCount++;
		}
		gettimeofday(&end, NULL);
		workloadETime += difftime(end - start);
		mallocCount = 0;
		freeCount = 0;
	}
	
	//f
	int iteration = 0;
	char *arr5[165];
	int size;
	int totalMalloc;
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, NULL);
		while(true){
			size = 1;
			for(ptr = (char*)malloc(size); ptr != NULL; ptr = (char*)malloc(size)){
				arr5[mallocCount] = ptr;
				mallocCount = (iteration == 1)? mallocCount + 2 : mallocCount + 1;
				size = size * 2;
			}
			iteration++;
			if(iteration == 2){
				break;
			}
			while(freeCount < mallocCount){
				free(arr5[freeCount]);
				freeCount += 2;
			}
			totalMalloc = mallocCount;
			mallocCount = 0;
		}
		freeCount = 0;
		while(freeCount < totalMalloc){
			free(arr5[freeCount]);
			freeCount++;
		}
		gettimeofday(&end, NULL);
		workloadFTime += difftime(end - start);
		iteration = 0;
		mallocCount = 0;
		freeCount = 0;
	}

	printf("Mean time to execute workload A %lf seconds\n", (workloadATime / 100));
	printf("Mean time to execute workload B %lf seconds\n", (workloadBTime / 100));
	printf("Mean time to execute workload C %lf seconds\n", (workloadCTime / 100));
	printf("Mean time to execute workload D %lf seconds\n", (workloadDTime / 100));
	printf("Mean time to execute workload E %lf seconds\n", (workloadETime / 100));
	printf("Mean time to execute workload F %lf seconds\n", (workloadFTime / 100));
*/
	printf("yerr\n");
	int *ptr = (int*)malloc(10);
	printf("(1)\n");
	free(ptr);

	int fakePtr;
	printf("(2)\n");
	free((int*)fakePtr);
	printf("returned\n");
}
