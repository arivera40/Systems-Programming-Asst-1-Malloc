#include<stdio.h>
#include<stddef.h>
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
	if(dataSize > sizeof(myblock)){		//p = (char*)malloc(4097)
		printf("Insufficient memory to fulfill malloc request\n filename: %s, Line: %d\n", file, line);
		return NULL;
	}
	if(dataSize == 0){
		return NULL;
	}
	if(head->next == NULL){
		initialize();
	}
	
	Metadata *ptr = head;
	Metadata *prevPtr;
	while(ptr != NULL){	//Runs loop to combine adjacent free blocks for best possible chance to allocate memory
		if(prevPtr != NULL){
			if(prevPtr->free == 1 && ptr->free == 1){
				combineBlocks(prevPtr, ptr);
				continue;
			}
			prevPtr = ptr;
			ptr = ptr->next;
		}
	}

	prevPtr = NULL;
	ptr = head;
	while(ptr != NULL){
		if(ptr->free == 1 && ptr->size > (dataSize + sizeof(Metadata))){
			insertMetadata(dataSize, ptr);
			return (void*)((void*)ptr + sizeof(Metadata));	//website: (void*)(++ptr) in order to return correct position in front
		}else if(ptr->free = 1 && ptr->size == dataSize){
			ptr->free = 0;
			return (void*)((void*)ptr + sizeof(Metadata));
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
	ptr = ptr->next;
	//Need to nullify the struct tempPtr is currently pointing to
	return;
}

//This function will be called everytime a new metadata structure needs to be inserted to bookmark end of requested data
void insertMetadata(size_t dataSize, Metadata *prevMetadata){
	Metadata *bookmark = (Metadata*)((void*)prevMetadata + dataSize + sizeof(Metadata));
	bookmark->size = prevMetadata->size - dataSize - sizeof(Metadata);
	bookmark->free = 1;
	if(prevMetadata->next == NULL){
		bookmark->next = NULL;
	}else{
		bookmark->next = prevMetadata->next;
	}
	prevMetadata->next = bookmark;
	prevMetadata->free = 0;
}

//free will set all data back to original null value and change corresponding metadata->free = 1
void myfree(void *ptr, int line, char *file){

	Metadata *prevMetadata = (Metadata*)((char*)ptr - sizeof(Metadata));
	printf("went in\n");
	if(prevMetadata == NULL){
		return;
	}
	printf("prevMetadata->size = %zu\n", prevMetadata->size); 
	// Freeing address that are not pointers, (1.a)
	//if(!(ptr exists in the region between the beginning of block array and end of block array))	(1.b)
		//The ptr was never allocated, throw error, Freeing pointers that were not allocated
	
	//The following code will work assuming 
/*	Metadata *prevMetadata = ptr;
	--prevMetadata;
	if(prevMetadata->free == 1){	//(1.c)
		//The data ahead is already free, throw error, redundant freeing
		printf("Redundant freeing: pointer has already been freed\nfilename: %s, line: %d\n", file, line);
	}
	prevMetadata->free = 1;	*/
	
}

