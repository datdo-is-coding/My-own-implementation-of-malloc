#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define BLOCK_SIZE sizeof(block_t)

#define ALIGN8(x) (((x) + 7) & ~7)

#define MINIMAL_SIZE 8

/*
    Why we need align?
    For struct not to break;
    Our struct block: contains size_t: 8 bytes, int: 4 bytes
    and a pointer: 8 bytes = 20 bytes
    We actually get 24 bytes because struct have padding so 
    that it occupied a multiple of 8 (because CPU loves it)
    So if the person use this memory space for a struct, we 
    need to align it not to make the struct break (aka the memory
    space that the struct takes will not be the memory space of 
    the next block)
*/

/*
    This struct represents the metadata for each block allocated
    size represents the size of the memory block
    free represents if the block is occupied
    next points to the next block
    The memory block will be right next to the block_t
*/

typedef struct block{
    size_t size;
    int free;
    struct block* next;
} block_t;

block_t* heap_head =  NULL;

/*
    Func: init_heap
    Behaviour:
    - Return a pointer to the memory user requested
    - If space not sufficient, return NULL
    - It is called when the heap first inintialized
*/

void* init_heap(size_t size){
    block_t* block = (block_t*) sbrk(BLOCK_SIZE + size);
    if(block == (void*) -1) return NULL;
    block -> size = size;
    block -> free = 0;
    block -> next = NULL;
    heap_head = block;
    return block + 1;
}
/*
    Func: my_malloc
    Behaviour:
    - Return a pointer to the memory user requested
    - If it is failed to allocate, return NULL
    - Algorithm: use Best fit to find the best chunk
    - If none found, create a new block with the requested size
    - If a block is found but it is too big, the block is split
*/
void* my_malloc(size_t size){
    size = ALIGN8(size);
    if(heap_head == NULL){
        return init_heap(size);
    }
    block_t* head = heap_head;
    block_t* best = NULL;
    while(head != NULL){
        if(head -> size >= size&&head ->free){
            if(best == NULL || best -> size > head -> size) best = head;
        }
        head = head -> next;
    }
    if(best != NULL){
        best -> free = 0;
        if(best -> size >= size + BLOCK_SIZE +MINIMAL_SIZE){
            char* split_addr = (char*)(best+1) + size;
            block_t* split_block = (block_t*) split_addr;
            split_block -> size = best -> size - size - BLOCK_SIZE;
            split_block -> free = 1;
            split_block -> next = best -> next;
            best -> next = split_block;
            best -> size = size;
        }

        return best + 1;
    }
    else {
        head = heap_head;
        while(head -> next != NULL){
            head = head -> next;
        }
        block_t* block = (block_t*) sbrk(BLOCK_SIZE + size);
        if(block == (void*) -1) return NULL;
        head -> next = block;
        block -> size = size;
        block -> free = 0;
        block -> next = NULL;
        return block + 1;
    }
}
/*
    Func: my_free
    Behaviour:
    - Set the block free state to 1;
    - Simple coalescing, if the current block and 
    next block is free, merge them into a bigger 
    block of memory
*/
void my_free(void* ptr){
    block_t* block = (block_t*)((char*)ptr - BLOCK_SIZE);
    block-> free = 1;  
    block_t* current = heap_head;
    while(current && current -> next){
        if(current -> free && current -> next -> free){
            current -> size += BLOCK_SIZE + current -> next -> size;
            current = current -> next ->next;
        } 
        else{
            current = current -> next;
        }
    }

}

int main(){
    char* hello = (char*)my_malloc(10);
    strcpy(hello, "hello");
    printf("%s\n",hello);

    my_free(hello);
    if(hello == NULL){
        printf("Memory freed");
    }
    else{
        printf("%s\n",hello);

    }
    return 0;
}