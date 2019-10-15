#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include"mymalloc.h"

Metadata *head = (void*) myblock;
//Initializes the head of the Metadata list
void initialize(){
	head->size = 4096 - sizeof(Metadata);	//The size of Metadata structure is accounted for by subtracting from entire size of myblock of memory
	head->free = 1;				//1 indicating the entire rest of myblock is free
	head->next = NULL;			//NULL as no other Metadata exists
}

//line and function parameters will be used to throw any errors that may occur
void *mymalloc(size_t dataSize, int line, char *file){
	printf("dataSize = %zu & sizeof(Metadata) = %zu\n", dataSize, sizeof(Metadata));
	if(dataSize > sizeof(myblock)){		//p = (char*)malloc(4097)
		printf("Insufficient memory to fulfill malloc request\n filename: %s, Line: %d\n", file, line);
		return NULL;
	}
	if(dataSize == 0){
		return NULL;
	}
	if(!(head->size)){
		initialize();
	//	printf("head->size = %zu\n", head->size);
	}
	
	Metadata *ptr = head;
	Metadata *prevPtr = NULL;
	while(ptr != NULL){	//Runs loop to combine adjacent free blocks for best possible chance to allocate memory
		if(prevPtr != NULL){
			if(prevPtr->free == 1 && ptr->free == 1){
				combineBlocks(prevPtr, ptr);
				ptr = ptr->next;
			}else{
				prevPtr = ptr;
				ptr = ptr->next;
			}
		}else{
			prevPtr = ptr;
			ptr = ptr->next;
		}
	}
	
	prevPtr = NULL;
	ptr = head;
/*	if(ptr->next == NULL){
		printf("head->next == NULL\nhead->free == 1\n");
	}else{
		printf("head->next == Metadata\nhead->free == %d\n", head->free);
	}*/
	while(ptr != NULL){
	//	printf("ptr->free == %d\n",head->free);
		if(ptr->free == 1 && ptr->size > (dataSize + sizeof(Metadata))){
	//		printf("does it enter here(1)\n");
	//		printf("Enters insertMetadata\n");
			insertMetadata(dataSize, ptr);
	//		printf("Exits insertMetadata\n");
	//		printf("head->size = %zu (after malloc)\n", head->size);
			return (void*)(++ptr);	//website: (void*)(++ptr) in order to return correct position in front
		}else if(ptr->free == 1 && ptr->size == dataSize){
			ptr->free = 0;
	//		printf("does it enter here(2)\n");
			return (void*)(++ptr);
		}
		prevPtr = ptr;
		ptr = ptr->next;
	}
	
	printf("Insufficient memory to fulfill malloc request\nfilename: %s, line: %d\n", file, line);
	return NULL;
}


void combineBlocks(Metadata *prevPtr, Metadata *ptr){
	prevPtr->next = ptr->next;
	prevPtr->size += ptr->size + sizeof(Metadata);
	Metadata *tempPtr = ptr;
	//Need to nullify the struct tempPtr is currently pointing to
	return;
}

//This function will be called everytime a new metadata structure needs to be inserted to bookmark end of requested data
void insertMetadata(size_t dataSize, Metadata *prevMetadata){
	Metadata *bookmark = (Metadata*)((void*)prevMetadata + dataSize + sizeof(Metadata));
	bookmark->size = prevMetadata->size - dataSize - sizeof(Metadata);
	printf("bookmark->size = %zu\n", bookmark->size);
	bookmark->free = 1;
	bookmark->next = prevMetadata->next;
	prevMetadata->next = bookmark;
	prevMetadata->size = dataSize;
	prevMetadata->free = 0;
}

//free will set all data back to original null value and change corresponding metadata->free = 1
void myfree(void *ptr, int line, char *file){

	Metadata *prevMetadata = ptr;
	--prevMetadata;

//	printf("prevMetadata->size = %zu\n", prevMetadata->size); 
	// Freeing address that are not pointers, (1.a)
	//if((char*)ptr < myblock || (char*)ptr >= myblock + 4096){printf("Pointer address is out of bounds\n file: %s, line: %d", file, line}	(1.b)
		//The ptr was never allocated, throw error, Freeing pointers that were not allocated
	
	//The following code will work assuming 
	if(prevMetadata->free == 1){	//(1.c)
		//The data ahead is already free, throw error, redundant freeing
		printf("Redundant freeing: pointer has already been freed\nfilename: %s, line: %d\n", file, line);
	}
	prevMetadata->free = 1;	
	
}

