#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This pointer that stores the address of the starting memory of the heap

void * heap_start = NULL;

/*
*	own_malloc
*   ---------------------
*	Behavior:
*   - Return pointer to the requested allocated memory
* 	Note:
*	- If sbrk fail to allocate memory, return NULL
*	- If it is the first time call sbrk it will assign the 
*	previous break to the heap start
*/

void* own_malloc(size_t size){
	void* allocated_memory = sbrk(size);
	if(allocated_memory == (void*) -1){
		return NULL;
	}
	if(heap_start == NULL){
		heap_start = allocated_memory;
	}
	return allocated_memory;
}
/*
*	free_all
*	Behaviour:
*	- Release allocated memory back to the OS
* 	Note:
*	- It is a demo, so this function is quite dangerous
*	It can wipe all the allocated memory
* 	- If the heap_start is NULL, return -1
*/
int free_all(){
	if(heap_start == NULL){
		return -1;
	}
	return brk(heap_start);
}

/*
* This program allocates a new memory and then free it
* and allocate a new memory again. It will overwrite the previous data
* of the first allocation.
*/

int main(){
	char* message = "hello this is my simplest malloc\0";
	size_t length = strlen(message) * sizeof(char);
	char* memory = (char*) own_malloc(length);
	strcpy(memory,message);
	printf("%s\n",memory);
	
	free_all();
	char* second_message = "this block is overwritten\0";
	size_t length_2 = strlen(second_message) *  sizeof(char);
	char* second_mem = (char*) own_malloc(length_2);
	strcpy(second_mem, second_message);
	printf("%s\n",second_mem);
	return 0;
}
