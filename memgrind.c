#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<time.h>
#include"mymalloc.h"

int randomize(int size){
	return 1 + (rand() % size);
}	

int main(int argc, char *argv){
	int workloadCount;

	int workloadATime = 0;
	int workloadBTime = 0;
	int workloadCTime = 0;
	int workloadDTime = 0;
	int workloadETime = 0;
	int workloadFTime = 0;

	int i;
	int j = 0;
	int k = 0;
	char *p;

	struct timeval start;
	struct timeval end;

	// A: malloc() 1 byte and immediately free it - do this 150 times
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, 0);
		for(i=0; i < 150; i++){	
			p = (char*)malloc(1);
			free(p);
		}
		gettimeofday(&end, 0);
		workloadATime += ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
	}


	// B: malloc() 1 byte, store the pointer in an array - do this 150 times.
 	// Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
	char *arr[150];
	int count = 0;
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, 0);
		for(i=0; i < 150; i++){	
			arr[i] = (char*)malloc(1);
			if(((i+1) % 50) == 0){			//possible error in if and following for loop condition !!!!!
				for(j=j; count < 50 ;j++){
					free(arr[j]);
					count++;
				}
				count = 0;
			}
		}
		gettimeofday(&end, 0);
		workloadBTime += ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		j=0;
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
		gettimeofday(&start, 0);
		while(freeCount != 50){
			//printf("freeCount = %d\nmallocCount = %d\n", freeCount, mallocCount);
			if(mallocCount == 50){
				free(arr2[freeCount]);
				freeCount++;
			}else{
				random = randomize(2);
				if(random == 1){	//malloc
					arr2[mallocCount] = (char*)malloc(1);
					mallocCount++;
				}else if(random == 2 && freeCount < mallocCount){	//free
					free(arr2[freeCount]);
					freeCount++;
				}else{		//malloc, because there isn't any current pointers to free
					arr2[mallocCount] = (char*)malloc(1);
					mallocCount++;
				}
			}
		}
		gettimeofday(&end, 0);
		workloadCTime += ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
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
	int options;

	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, 0);
		while(freeCount != 50){
			if(mallocCount == 50){
				free(arr3[freeCount]);
				freeCount++;
			}else{
				options = randomize(2);
				if(options == 1){	//malloc
					random = randomize(64);
					//printf("random = %d", random);
					arr3[mallocCount] = (char*)malloc(random);
					mallocCount++;
				}else if(options == 2 && freeCount < mallocCount){	//free
					free(arr3[freeCount]);
					freeCount++;	
				}else{
					random = randomize(64);
					arr3[mallocCount] = (char*)malloc(random);
					mallocCount++;
				}
			}
		}
		gettimeofday(&end, 0);
		workloadDTime += ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
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
		gettimeofday(&start, 0);
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
			arr4[mallocCount] = ptr;
			mallocCount++;
		}
		while(freeCount < mallocCount){
			free(arr4[freeCount]);
			freeCount++;
		}
		gettimeofday(&end, 0);
		workloadETime += ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		mallocCount = 0;
		freeCount = 0;
	}
	
	//f
/*	int iteration = 0;
	char *arr5[165];
	int size;
	int totalMalloc;
	for(workloadCount = 0; workloadCount < 100; workloadCount++){
		gettimeofday(&start, 0);
		while(iteration != -1){
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
		gettimeofday(&end, 0);
		workloadFTime += ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		iteration = 0;
		mallocCount = 0;
		freeCount = 0;
	}*/

	char **arraytest = (char**)malloc(sizeof(char*) * 3);
	for(i=0; i < 3; i++){
		arraytest[i] = (char*)malloc(sizeof(char) * 3);
	}

	printf("Average time to execute workload A: %d milliseconds\n", (workloadATime / 100));
	printf("Average time to execute workload B: %d milliseconds\n", (workloadBTime / 100));
	printf("Average time to execute workload C: %d milliseconds\n", (workloadCTime / 100));
	printf("Average time to execute workload D: %d milliseconds\n", (workloadDTime / 100));
	printf("Average time to execute workload E: %d milliseconds\n", (workloadETime / 100));
//	printf("Average time to execute workload F: %d milliseconds\n", (workloadFTime / 100));

/*	printf("starts here...\n");
	printf("Enters malloc function\n");
	int *ptr = (int*)malloc(2);
	printf("Exits malloc function\n");
	printf("Enters free function\n");
	free(ptr);
	printf("Exits free function\n");

	int fakePtr;
	printf("Enters free function(with fake pointer)\n");
	free((int*)fakePtr);
	printf("Exits free function(with fake pointer)\n");	*/
}
