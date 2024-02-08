#ifndef A3SUPPORT_INCLUDED
#define A3SUPPORT_INCLUDED
#include <sys/types.h>
////////////////////////////////////////////////////////////////////////////
//
//  File          : a2support.h
//  Description   : This is the header file for the functions for assignment
//                  1 of the CMSC257 course.  Students are required to define
//                  the following functions and implement them in another
//                  file, a1support.c.
//
//  Author        : Christopher Flippen
//  Created       : 2/20/19

//Functions to define

//In your implementation, ensure all allocated blocks are of a size that is a multiple of 8.
struct block_meta 
{
	size_t size;
	struct block_meta *next;
	struct block_meta *prev; //Making doubly linked
	int free;
	//int magic; // For debugging only. TODO: remove this in non-debug mode.
};
          
struct block_meta *find_best_block(struct block_meta **last, size_t size);
struct block_meta *find_free_block(struct block_meta **last, size_t size);
struct block_meta *request_space(struct block_meta* last, size_t size);
void *malloc(size_t size);
void split(int neededSize, int blockSize, struct block_meta *block);
void  merge(struct block_meta *block);
void *calloc(size_t nitems, size_t size);
void *realloc(void *ptr, size_t size);
struct block_meta *get_block_ptr(void *ptr);
void free(void *ptr);
int findExternalLeak();
#endif
