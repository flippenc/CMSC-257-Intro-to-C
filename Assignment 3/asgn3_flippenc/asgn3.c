////////////////////////////////////////////////////////////////////////////////
//
//  File          : asgn3.c
//  Description   : The main file (asgn3.c) should contain a driver which tests your code.
//
//  Author        : Christopher Flippen
//  Created       : 3/12/19
//

// Project Includes
// Include Files
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "asgn3_support.h"
//include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

//////////////////////////////////////////////////////////////////////////
//
// Function     : printPointers(struct block_meta *checkPointer)
// Description  : print pointer to the given block_meta pointer, used for testing
//
// Inputs       : a block_meta pointer
// Outputs      : print the address of the given block_meta pointer
//
void printPointers(struct block_meta *checkPointer)
{
	printf("%x\n",get_block_ptr(checkPointer));
}

////////////////////////////////////////////////////////////////////////////
//
// Function     : findInternalLeak(int alloc)
// Description  : Find the difference between the allocated memory size and the nearest multiple of 8 (rounding up)
//
// Inputs       : an int for the amount of memory being allocated my malloc
// Outputs      : the difference between the allocated memory size and the nearest multiple of 8 (rounding up)
//
int findInternalLeak(int alloc)
{
	int leaked = 0;
	while (alloc % 8 != 0)
	{
		leaked++;
		alloc++;
	}
	return leaked;	
}


////////////////////////////////////////////////////////////////////////////
//
// Function     : main( void )
// Description  : Driver method to test and demonstrate code
// 		  1)    Make at least 10 calls to the malloc() function.
// 		  2)    Make at least 10 calls to the calloc() function.
// 		  3)    Make at least 10 calls to the realloc() function.
// 		  4)    Make at least 5 calls to the free() function.
// 		  5)    Print the total memory leakage of your driver running your implementation. 
// 		  	Memory leakage is defined as memory which is allocated, but not used.
// 		  6)    Print the starting and ending addresses of the heap
//
// Inputs       : none
// Outputs      : return code 1
//
int main ( void )
{
	void* start = sbrk(0);
	////////////////////////////////////////////////////////////////////////////////////
	//To get start of the heap, sbrk(0) at start of program, to get end, sbrk(0) at end/
	////////////////////////////////////////////////////////////////////////////////////
	int internalLeak = 0;
	int externalLeak = 0;
 	struct block_meta *test1 = (struct block_meta *)malloc(sizeof(struct block_meta));
	struct block_meta *test2 = (struct block_meta *)malloc(sizeof(struct block_meta));
	internalLeak += findInternalLeak(sizeof(struct block_meta));
	struct block_meta *test3;
	struct block_meta *test4;
	test4=(struct block_meta *)calloc(4,sizeof(struct block_meta));
	internalLeak += findInternalLeak(4*sizeof(struct block_meta)); 
	test3=realloc(test2,2*sizeof(struct block_meta));
	externalLeak += findExternalLeak();
	free(test4);
	externalLeak +=findExternalLeak();
	struct block_meta *test5 = (struct block_meta *)malloc(sizeof(struct block_meta));
	free(test5);
	struct block_meta *test6;
	struct block_meta *test7;
	test6=(struct block_meta *)calloc(4,sizeof(struct block_meta));
	free(test6);
	free(test7);

	struct block_meta *test10 = (struct block_meta *)malloc(55);
	internalLeak += findInternalLeak(55);
	struct block_meta *test11 = (struct block_meta *)malloc(66);
	internalLeak += findInternalLeak(66);
	struct block_meta *test12 = (struct block_meta *)malloc(77);
	internalLeak += findInternalLeak(77);
	struct block_meta *test13 = (struct block_meta *)malloc(88);
	internalLeak += findInternalLeak(88);
	struct block_meta *test14 = (struct block_meta *)malloc(99);
	internalLeak += findInternalLeak(99);
	struct block_meta *test15 = (struct block_meta *)malloc(111);
	internalLeak += findInternalLeak(111);
	struct block_meta *test16 = (struct block_meta *)malloc(222);
	internalLeak += findInternalLeak(222);
	struct block_meta *test17 = (struct block_meta *)malloc(333);
	internalLeak += findInternalLeak(333);
	struct block_meta *test18 = (struct block_meta *)malloc(444);
	internalLeak += findInternalLeak(444);
	struct block_meta *test19 = (struct block_meta *)malloc(555);
	internalLeak += findInternalLeak(555);
	struct block_meta *test20 = (struct block_meta *)malloc(55);
	internalLeak += findInternalLeak(55);
	struct block_meta *test21 = (struct block_meta *)malloc(66);
	internalLeak += findInternalLeak(66);
	struct block_meta *test22 = (struct block_meta *)malloc(77);
	internalLeak += findInternalLeak(77);
	
	test12 = realloc(test10,2*sizeof(struct block_meta));
        internalLeak -= findInternalLeak(55);
        internalLeak += findInternalLeak(2*sizeof(struct block_meta));

	test13 = realloc(test11,3*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(66);
	internalLeak += findInternalLeak(3*sizeof(struct block_meta));

	test14 = realloc(test12,4*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(77);
	internalLeak += findInternalLeak(4*sizeof(struct block_meta));

	test15 = realloc(test13,5*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(88);
	internalLeak += findInternalLeak(5*sizeof(struct block_meta));	

	test16 = realloc(test14,6*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(99);
	internalLeak += findInternalLeak(6*sizeof(struct block_meta));

	test17 = realloc(test15,7*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(111);
	internalLeak += findInternalLeak(7*sizeof(struct block_meta));

	test18 = realloc(test16,8*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(222);
	internalLeak += findInternalLeak(8*sizeof(struct block_meta));

	test19 = realloc(test17,9*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(333);
	internalLeak += findInternalLeak(9*sizeof(struct block_meta));

	test20 = realloc(test18,10*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(444);
	internalLeak += findInternalLeak(10*sizeof(struct block_meta));

	test21 = realloc(test19,11*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(55);
	internalLeak += findInternalLeak(11*sizeof(struct block_meta));
	
	test22 = realloc(test20,5*sizeof(struct block_meta));
	internalLeak -= findInternalLeak(66);
	internalLeak += findInternalLeak(5*sizeof(struct block_meta));

	test12=(struct block_meta *)calloc(5,30);
	internalLeak += findInternalLeak(5*30);
	test13=(struct block_meta *)calloc(6,20);
	internalLeak += findInternalLeak(6*20);
	test14=(struct block_meta *)calloc(5,42);
	internalLeak += findInternalLeak(5*42);
 	test15=(struct block_meta *)calloc(7,99);
	internalLeak += findInternalLeak(7*99);
 	test16=(struct block_meta *)calloc(3,87);
	internalLeak += findInternalLeak(3*87);
	test17=(struct block_meta *)calloc(2,63);
	internalLeak += findInternalLeak(2*63);
	test18=(struct block_meta *)calloc(4,51);
	internalLeak += findInternalLeak(4*51);
	test19=(struct block_meta *)calloc(2,73);
	internalLeak += findInternalLeak(2*73);
	test20=(struct block_meta *)calloc(12,21);
	internalLeak += findInternalLeak(12*21);
	test21=(struct block_meta *)calloc(10,23);
	internalLeak += findInternalLeak(10*23);
	test22=(struct block_meta *)calloc(16,91);
	internalLeak += findInternalLeak(16*91);

	free(test17);
	free(test16);
	free(test15);
	free(test18);
	free(test14);

	externalLeak += findExternalLeak();
	printf("The heap starts at: ");
	printf("%d\n", start);
	printf("The heap ends at: ");
	printf("%d\n",sbrk(0));
	printf("Internal leak: ");
	printf("%d\n", internalLeak);
	printf("External leak: ");
	printf("%d\n", externalLeak);
	return(1);
}
