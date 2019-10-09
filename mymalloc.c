#include<stdio.h>
#include<stddef.h>
#include<mymalloc.h>

//Initializes the head of the Metadata list
void initialize(){
	head->size = 4096 - sizeof(Metadata);	//The size of Metadata structure is accounted for by subtracting from entire size of myblock of memory
	head->free = 1;				//1 indicating the entire rest of myblock is free
	head->next = NULL;			//NULL as no other Metadata exists
}

//line and function parameters will be used to throw any errors that may occur
void *mymalloc(size_t dataSize, int line, char *function){
	Metadata *ptr = head;
	size_t totalFreeData = 0; 
	if(head->next == NULL){
		initialize();
	}
	while(ptr != NULL){
		if(ptr->free == 1 && ptr->size >= (dataSize + sizeof(Metadata)){
			insertMetadata(dataSize, ptr);
			return (void*)((void*)ptr + sizeof(Metadata));	//believe should return pointer in position right in front of metadata
		}else if(ptr->free = 1){
			totalFreeData += ptr->size;
		}
		ptr = ptr->next;
	}
	if(totalFreeData < (dataSize + sizeof(Metadata)){
		//There is not enough space
		//return some type of error
	}
	//otherwise code will continue, it should combine existing free blocks 
	
}

//In the case that the existing free blocks are too small we will combine free blocks to make them larger in order to fulfill malloc request
//*** HARD FUNCTION TO CODE ***
void combineBlocks(){

}

//This function will be called everytime a new metadata structure needs to be inserted to bookmark end of requested data
void insertMetaData(size_t dataSize, Metadata *prevMetadata){
	Metadata bookmark = (void*)((void*)prevMetadata + dataSize + sizeof(Metadata));
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
void myfree(void *ptr){

}

