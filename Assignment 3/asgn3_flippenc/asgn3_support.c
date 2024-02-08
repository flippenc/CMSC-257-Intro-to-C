//////////////////////////////////////////////////////////////////////////////
//
//  File          : asgn3_support.c
//  Description   : Functions to simulates major memory management functions in C
//
//  Author        : Christopher Flippen
//  Created       : 3/11/19
//

// Project Includes
// Include Files
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "asgn3_support.h"
#include <stdio.h>
//include <stdlib.h>

#define META_SIZE sizeof(struct block_meta)

//Head for linked list
void *global_base = NULL;

//////////////////////////////////////////////////////////////////////////////
//
// Function     : find_free_block(struct block_meta **last, size_t size)
// Description  : Find the free block which best fits for holding size amount of data
//
// Inputs       : a block_meta pointer, last which will store the block before the best fit block
//                an size_t respresenting the amount of memory needed in the block
// Outputs      : a pointer to a block_meta which best fits the desired amount of memory
//
struct block_meta *find_free_block(struct block_meta **last, size_t size)
{
	///////////////////////////////////////////////////////////////////
	//current stores the current block while scanning through the list/
	//best stores the block pointer with the size closest to size     /
	///////////////////////////////////////////////////////////////////
        struct block_meta *current = global_base;
	struct block_meta *best = global_base;
	///////////////////////////////////////////////////////////////////
	//Setting best's size to a default value to compare block sizes to/
	///////////////////////////////////////////////////////////////////
	best->size = current->size+1;
	//////////////////////////////////////////////////////////////////////////
	//While there are more nodes in the list, keep iterating through         /
	//If a free block is found with a size smaller than the previous best and/
	//large enough to be usable, set best to the current block               /
	//////////////////////////////////////////////////////////////////////////
        while (current)
        {
		if ((current->free) && (current->size >= size))
                {
                        if (current->size < best->size)
                        {
                                best = current;
                        }
                }
		*last = current;
		current = current->next;

        }
	////////////////////////////////////////////////////////////////////////////////////////////
	//If the loop completed without finding a block, current will be NULL                      /
	//Last points to either the last block in the list or the block right before the free block/
	////////////////////////////////////////////////////////////////////////////////////////////
	*last = best;
        return current;
}


////////////////////////////////////////////////////////////////////////////
//
// Function     : request_space(struct block_meta* last, size_t size)
// Description  : If find_free_block doesn't find a block, the program needs to request space from the OS using sbrk
// 		  and adding the new block to the end of the linked list
//
// Inputs       : a pointer to the block_meta at the end of the current list
//                an size_t respresenting the amount of memory needed in the block
// Outputs      : a block_meta pointer to the requested space
//
struct block_meta *request_space(struct block_meta* last, size_t size)
{
	struct block_meta *block;
	///////////////////////////////////////////////////////////
	//Find current top of heap, ignored previously freed space/
	///////////////////////////////////////////////////////////
	block = sbrk(0);
	////////////////////////////////////////////////////////////
	//A block takes up its size variable + the blocks META_SIZE/
	////////////////////////////////////////////////////////////
	void *request = sbrk(size + META_SIZE);
	//////////////////////////////////////////////
	//Checking that the block is the correct size/
	//////////////////////////////////////////////
	assert((void*)block == request);
	if (request == (void*) -1)
	{
		/////////////////////////////
		//If sbrk fails, return NULL/
		/////////////////////////////
		return NULL;

	}
	/////////////////////////////////////////////////////////////
	//if (last) returns true if last is not NULL                /
	//if last, link the block to the correct blocks in the chain/
	/////////////////////////////////////////////////////////////
	if (last)
	{
		block->prev = last;
		last->next = block;
	}
	//////////////////////////////////
	//Setting variables for the block/
	//////////////////////////////////
	block->size = size;
	block->next = NULL;
	block->free = 0;
	return block;
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : find_free_block(struct block_meta **last, size_t size)
// Description  : Allocate a segment of memory in the heap, of at least the given size (rounds up to nearest multiple of 8)
// 		  and return a pointer to this segment
//
// Inputs       : a size_t value size for the block to be malloced
// Outputs      : void
//
void *malloc(size_t size)
{
	//////////////////////////////////////
	//Aligning size to be a multiple of 8/
	//////////////////////////////////////
	while (size % 8 != 0)
	{
		size++;
	}
	struct block_meta *block;
	////////////////////////////////////////////////////
	//If trying to allocate negative space, return NULL/
	////////////////////////////////////////////////////
	if (size <= 0)
	{
		return NULL;
	}
	///////////////////////////////////////////////
	//If global_base is NULL, it is the first call/
	///////////////////////////////////////////////
	if (!global_base)
	{
		////////////////////////////////////////////////////////////////////////////////////
		//If no space allocated, then there is no last and request_space will create a last/
		//If space can not be created, return NULL                                         /
		////////////////////////////////////////////////////////////////////////////////////
		block = request_space(NULL, size);
		if (!block)
		{
			return NULL;
		}
		global_base = block;
	}
	else
	{
		struct block_meta *last = global_base;
		block = find_free_block(&last, size);
		////////////////////////////////////////////////////////////////
		//If no free block found, request_space of the appropriate size/
		////////////////////////////////////////////////////////////////
		if (!block)
		{
			block = request_space(last, size);
			///////////////////////////////////////////
			//If space can not be created, return NULL/
			///////////////////////////////////////////
			if (!block)
			{
				return NULL;
			}
		}
		///////////////////////////////////////////////////////////////////
		//If free block found, split it to use the correct amount of space/
		///////////////////////////////////////////////////////////////////
		else
		{
			split(size, block->size, block);
			block->free = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Returns a pointer to the region after block_meta, +1 increments by 1 sizeof(struct(block_meta))/
	//////////////////////////////////////////////////////////////////////////////////////////////////
	return(block+1);
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : split(int neededSize, int blockSize, struct block_meta *block)
// Description  : Split block into two blocks, one with the needed space for allocation and the other for the extra free space
//
// Inputs       : int neededSize - size needed in the block
// 		  int blockSize - size of the block parameter, not really needed since it is equal to block->size
// 		  struct block_meta *block - pointer to a block_meta, this is the block that will be split into 2 blocks
// Outputs      : void
//
void split(int neededSize, int blockSize, struct block_meta *block)
{
	////////////////////////////////////////////////////////////////////////
	//If neededSize is the same size as the block trying to be split return/
	////////////////////////////////////////////////////////////////////////
	if (neededSize == blockSize)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	//usedBlock stores the amount of memory as designated by neededSize and stores it after block/
	//block is the block that is being used, freeBlock is the block that will be free            /
	//////////////////////////////////////////////////////////////////////////////////////////////
	struct block_meta *freeBlock = block +1;
	block->next=freeBlock;
	freeBlock->size=blockSize-neededSize;
	block->size=neededSize;
	freeBlock->next=block->next;
	block->next=freeBlock;
	freeBlock->prev=block;
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : find_free_block(struct block_meta **last, size_t size)
// Description  : Given a newly freed memory block, check if any of its neighboring blocks are also free and combine them accordingly
// 		  Will continue merging block until there are no adjacent free blocks
//
// Inputs       : a pointer to a block_meta that has been freed
// Outputs      : void
//
void merge(struct block_meta *block)
{
	//////////////////////////////////////////////////////////
	//newSize used to store the new size of the merged blocks/
	//////////////////////////////////////////////////////////
	int newSize = -1;
	struct block_meta *previous = block->prev;
	struct block_meta *nextOne = block->next;
	/////////////////////////////////////////////////////
	//while block->prev exists and is free, merge blocks/
	/////////////////////////////////////////////////////
	while (block->prev && block->prev->free)
	{
		newSize = block->size + previous->size + sizeof(struct block_meta);
		previous->size=0;
		block->size= newSize;
		if (block->prev->prev)
		{
			block->prev=previous->prev;
			previous->prev->next=block;
			previous = block->prev;
		}
		else
		{
			block->prev=NULL;
		}
	}
	////////////////////////////////////////////////////
	//while block->next exists and is free merge blocks/
	////////////////////////////////////////////////////
	while (block->next && block->next->free)
        {
               	newSize = block->size + nextOne->size + sizeof(struct block_meta);
        	nextOne->size=0;
               	block->size= newSize;
              	if (block->next->next)
               	{
                       	block->next=nextOne->next;
                       	nextOne->next->prev=block;
                       	nextOne = block->next;
               	}
               	else
               	{
                       	block->next=NULL;
               	}
       	}
}

//////////////////////////////////////////////////////////////////////////////////
//
// Function     : calloc(size_t nitems, size_t size)
// Description  : Allocate a segment of memory in the heap to store nitems elements of the given size
// 		  Set the entire segment of memory to 0. Return a pointer to this segment
//
// Inputs       : size_t nitems - the number of items to be created
// 		  size_t size - the size of each item
// Outputs      : a void pointer to the malloced blocks
//
void *calloc(size_t nitems, size_t size)
{
	///////////////////////////////////////////
	//Clears memory before returning a pointer/
	///////////////////////////////////////////
	size_t totalSize = nitems * size;
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : realloc(void *ptr, size_t size)
// Description  : Resize the memory segment pointed to by ptr to the given size
// 		  Return the pointer to the newly sized segment
//
// Inputs       : a void pointer to a memory location to be reallocated
// 		  a size_t size for the new size of the memory location
// Outputs      : a void pointer to the reallocated memory
//
void *realloc(void *ptr, size_t size)
{
	if(!ptr)
	{
		/////////////////////////////////////////////////
		//If ptr is NULL, realloc should act like malloc/
		/////////////////////////////////////////////////
		return malloc(size);
	}
	
	struct block_meta* block_ptr = get_block_ptr(ptr);
	if (block_ptr->size >= size)
	{
		////////////////////////////////////////
		//We have enough space, split the block/
		////////////////////////////////////////
		split(size,block_ptr->size,block_ptr);
		return ptr;
	}
	//////////////////////////////////////////////////////////////
	//Need to really realloc. Malloc new space and free old space/
	//Then copy old data to new space                            /
	//////////////////////////////////////////////////////////////
	void *new_ptr;
	new_ptr = malloc(size);
	if (!new_ptr)
	{
		return NULL;
	}
	memcpy(new_ptr, ptr, block_ptr->size);
	free(ptr);
	return new_ptr;
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : get_block_ptr(void *ptr)
// Description  : Getting the pointer for the block_meta at the location ptr points to
//
// Inputs       : void *ptr - the memory location to get a block_meta from
// Outputs      : the block_meta at *ptr's location
//
struct block_meta *get_block_ptr(void *ptr)
{
	return (struct block_meta*)ptr -1;
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : free(void *ptr)
// Description  : Deallocate the memory segment pointed to by ptr and merge any adjacent free blocks to the newly freed block
//
// Inputs       : a void pointer to a memory location to free
// Outputs      : void
//
void free(void *ptr)
{
	//////////////////////////////////////////////////////////////////////////////
	//If the pointer is NULL, don't do anything since the memory is already freed/
	//////////////////////////////////////////////////////////////////////////////
	if (!ptr)
	{
		return;
	}
	////////////////////////////////////
	//Merge blocks after they are freed/
	////////////////////////////////////
	struct block_meta* block_ptr = get_block_ptr(ptr);
	if (block_ptr==NULL)
	{
		return;
	}
	merge(block_ptr);
	assert(block_ptr->free == 0);
	block_ptr->free = 1;	
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : findExternalLeak()
// Description  : Scan through the current linked list of memory, keeping track of the amount of external memory leaks
// 		  External memory leak is sum of the size values of each free block in the list
//
// Inputs       : none
// Outputs      : an int for the amount of externally leaked memory
//
int findExternalLeak()
{
	/////////////////////////////////////////////////////////
	//externalLeak used to keep track of memory leaked      /
	//findFree used to scan through the list                /
	//while the blocks exist, keep scanning through the loop/
	//If the block is free, add its size to externalLeak    /
	/////////////////////////////////////////////////////////
	int externalLeak = 0;
	struct block_meta *findFree = global_base;
	while (findFree && findFree->next)
	{
		if (findFree->free)
		{
			externalLeak+=findFree->size;
		}
		findFree=findFree->prev;
	}
	return externalLeak;
}
