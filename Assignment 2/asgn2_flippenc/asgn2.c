////////////////////////////////////////////////////////////////////////////////
//
//  File          : cmsc257-s16-assign2.c
//  Description   : This is the main code file for the CMSC257 assignment 2.
//                  see class assignment for details on what needs to be added.
//
//  Author        : Christopher Flippen
//  Created       : 2/11/19
//


// Include Files


// Project Includes
#include "a2support.h"
// Include Files
#include <stdlib.h>
#include <stdio.h>
// Defines
#define NUMBER_ENTRIES 20

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : This is the main function for the cmsc257-s16-assign2 program.
//
// Inputs       : none
// Outputs      : 0 if successful, -1 otherwise


int main( void ) {
	///////////////////
	// Local variables/
	///////////////////
	float f_array[NUMBER_ENTRIES];
	int i, i_array[NUMBER_ENTRIES];
	///////////////////////////////////////////////////////////////////////////////////
	//Read in 20 float values from the user, one per line, and place them in an array./
	///////////////////////////////////////////////////////////////////////////////////
	printf("Input 20 floats: ");
	printf("\n");
	for ( i=0; i<NUMBER_ENTRIES; i++ )
	{
		scanf( "%f", &f_array[i] );
	}
	///////////////////////////////////////////////////////////////////////////////
	//a is a counter variable, b is used to hold and work with values from f_array/
	///////////////////////////////////////////////////////////////////////////////
	int a;
	int b;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Create a second array of 20 integers. Populate this array by converting the floats to ints as follows:/
	//	a)	Truncate the float to an integer                                                        /
	//	b)	Convert the int to a positive value by taking it’s absolute value                       /
	//	c)	Convert the int to the range 0 to 15 inclusive by using the modulus operation           /
	//https://www.geeksforgeeks.org/compute-the-integer-absolute-value-abs-without-branching/               /
	//Used Method 1 for finding the absolute value using bitwise                                            /
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for ( a = 0; a<NUMBER_ENTRIES; a++)
	{
		b = f_array[a];
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Bit shift, used for removing the sign bit to take absolute value, int is 32 bits so the shift removes the sign bit            /     
		//XOR of mask+b gives absolute value                                                                                            /
		//Getting from negative 2's complement by adding 1 and inverting the bits. Since the numbers aren't always positive or negative,/
		//removing the sign bit and XORing works for negative or positive                                                               /
        	//https://stackoverflow.com/questions/3072665/bitwise-and-in-place-of-modulus-operator                                          /
        	//I used polygenelubricants' response of using the AND operator to take modulo of numbers                                       /
        	//This method works for just positive numbers, but since the numbers are all positive, this will work                           /
        	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int mask = b>>31;
		b = (mask+b)^mask;
		/////////
		//Mod 16/
		/////////
		b = b&15;
		i_array[a] = b;
	}
	printf("\n");
	////////////////////////////////////////////////////////
	//Print the contents of the float array to the terminal/
	////////////////////////////////////////////////////////
	printf("Values in float array ");
	float_display_array(f_array, NUMBER_ENTRIES);
	printf("\n");
	//////////////////////////////////////////////////////////
	//Print the contents of the integer array to the terminal/
	//////////////////////////////////////////////////////////
	printf("Values in int array ");
	integer_display_array(i_array, NUMBER_ENTRIES);
	printf("\n");
	int g; 
	//////////////////////////////////////////////////////////////////////////////
	//g is a counter variable for scanning through i_array                       /
	//For each integer, print the number of "1" bits in its binary representation/
	//////////////////////////////////////////////////////////////////////////////
	for (g = 0; g<NUMBER_ENTRIES; g++)
	{
		printf("%i",i_array[g]);
		printf(" has ");
		printf("%i", count_bits(i_array[g]));
		printf(" ones in its binary representation");
		printf("\n");
	}
	//////////////////////////////////////////////////////////
	//6)	Sort the integer array using the quicksort method/
	//////////////////////////////////////////////////////////
	printf("Sorted the integer array.");
	integer_quicksort(i_array, 0, NUMBER_ENTRIES-1);
	printf("\n");
	////////////////////////////////////////////////////////
	//7)	Print the sorted integer array to the terminal./
	////////////////////////////////////////////////////////
	printf("Sorted integer array: ");
	integer_display_array(i_array, NUMBER_ENTRIES);
	printf("\n");
	////////////////////////////////////////////////////////////
	//8)	Print the number of even values in the float array./
	////////////////////////////////////////////////////////////
	printf("Number of evens in float array: ");
	printf("%d",float_evens(f_array, NUMBER_ENTRIES));
	printf("\n");
	//////////////////////////////////////////////////////////////
	//9)	Print the number of even values in the integer array./
	//////////////////////////////////////////////////////////////
	printf("Number of evens in integer array: ");
	printf("%d",integer_evens(i_array, NUMBER_ENTRIES));
	printf("\n");
	///////////////////////////////////////////////////////////////
	//10)   Print the most frequent value(s) in the integer array./
	///////////////////////////////////////////////////////////////
	most_values(i_array, NUMBER_ENTRIES, 15);
	////////////////////////////////////////////////////////////////////////////
	//11)	 For each integer in the sorted integer array:                     /
	//	a)	Cast the int to an unsigned short int.                     /
	//	b)	Print the binary representation of this unsigned short int./
	//	c)	Reverse the bits of this binary representation.            /
	//	d)	Print the reversed binary representation.                  /
	//	e)	Print the value of the reversed binary representation.     /
	//		(Convert it back to decimal).                              /
	////////////////////////////////////////////////////////////////////////////
	unsigned short int u_array[NUMBER_ENTRIES];
	////////////////////////////////////////////////////////////
	//Char array used to hold bits of the values               /
	//Unsigned short ints are 2 bytes long                     /
	//The last element of the array holds the string terminator/
	////////////////////////////////////////////////////////////
	char char_array[17];
	for (g = 0; g<NUMBER_ENTRIES; g++)
	{
		/**a*/u_array[g]=(unsigned short int)i_array[g];
		///////////////////////////////////////
		//Unsigned short ints are 2 bytes long/
		///////////////////////////////////////
		printf("\n");
		printf("The unsigned short value ");
		printf("%i", u_array[g]);
		printf(":\nORIG: ");
		/**b*/binary_string(u_array[g], char_array, 16);
		printf("\n");
		/**c, d, and e*/reverse_bits(u_array[g]);
	}
	///////////////////////
	// Return successfully/
	///////////////////////
	printf("\n");
	return(0);
}
