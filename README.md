# SIMPLE MALLOC IMPLEMENTATION

# DATA STRUCTURE

I am using a struct to describe the metadata about the memory space that user asks for.
I used a linked list for the metadata.
```
struct block_t{
  size_t size;
  int    free;
  struct block_t* next;
}
```
**size**: how many bytes this block occupied
**free**: is this block used
**next**: points to the next block

We need a variable to store the head of the heap.

# How it works

In order to mimic how malloc works, we will use ```brk()``` and ```sbrk()``` to get the newly allocated memory from the OS.
I recommend you to read the manual for ```brk()``` and ```sbrk()```.

## malloc
Usage: provide how many bytes you need, it will return the pointer of that memory space. Return NULL if allocation failed.
Behaviour:
- If the heap is not initialized, it creates a new block and assigns the heap head to the memory address of the block
- Traverse through the linked list to find suitable block. If there is no suitable block, it creates new block at the end.
- If there is suitable block, we check if the block size is too big. We will split the block to create another block.
- Return the address of the memory block

## free
Usage: provide the address of the block. Change the **free** state to 1.
Behaviour:
- If the provided pointer is NULL, return.
- Change the **free** state of the current block.
- Traverse from the heap start to the end, we check if the current and the next block are free.
If they are free, merge the two block toghther -> Basic coalescing.

**NOTE** : this is for the sake of simplicity to understand how malloc works behind the scence.
