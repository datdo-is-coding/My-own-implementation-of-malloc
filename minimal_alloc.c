#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * heap_start = NULL;

void* own_malloc(size_t size){
	void* allocated_memory = sbrk(size);
	if(heap_start == NULL){
		heap_start = allocated_memory;
	}
	return allocated_memory;
}

int free_all(){
	if(heap_start == NULL){
		return -1;
	}
	return brk(heap_start);
}

int main(){
	char* message = "hello this is my simplest malloc\0";
	size_t length = strlen(message) * sizeof(char);
	char* memory = (char*) own_malloc(length);
	strcpy(memory,message);
	printf("%s\n",memory);
	
	free_all();
	char* second_message = "this block is overwritten\0";
	size_t length_2 = sizeof(second_message) *  sizeof(char);
	char* second_mem = (char*) own_malloc(length_2);
	strcpy(second_mem, second_message);
	printf("%s\n",second_mem);
	return 0;
}
