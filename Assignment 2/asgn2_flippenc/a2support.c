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
 #include <stdio.h>
 #include <stdlib.h>
//

// Project Includes
#include "a2support.h"

////////////////////////////////////////////////////////////////////////////////
//
// Function     : float_display_array
// Description  : Prints the values of an array of floats to the terminal on a single line.
//                Delimit these values with a single space.
//                Only print the first two values to the right of the decimal point.
//		  
// Inputs       : a reference to a float array
//                the length of the array as an int
// Outputs	: void
//
void float_display_array(float f_array[], int length)
{
	////////////////////////////////////////////////////////
	//c is a counter variable for the loop                 /
	////////////////////////////////////////////////////////
	int c;
	for ( c = 0; c<length; c++)
        {
                printf("%.2f",f_array[c]);
                printf(" ");
        }
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : integer_display_array
// Description  : Prints the values of an array of ints to the terminal on a single line
//		  Delimit these values with a single space
//
// Inputs       : A reference to an int array.
//                The length of the array as an int
//
// Outputs      : void
//
void integer_display_array(int i_array[], int length)
{
	
	////////////////////////////////////////////////////////
	//f is a counter variable for the loop                 /
	////////////////////////////////////////////////////////
	int f; 
        for (f = 0; f<length; f++)
        {
                printf("%i",i_array[f]);
                printf(" ");
        }
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : float_evens
// Description  : Return the number of even floats in the array
// 		  Ignore anything to the right of the decimal place
//
// Inputs       : A reference to a float array
//                The length of the array as an int
//
// Outputs      : an int for the number of even floats in the array
//
int float_evens(float f_array[], int length)
{
	///////////////////////////////////////////////////////
	//a is a counter variable to look for even values     /
	//b stores the values in the float array              /
	//numEvens tracks how many even numbers are in f_array/
	///////////////////////////////////////////////////////
	int a; 
	int b; 
	int numEvens = 0;
	for (a = 0; a<length; a++)
	{
		b = (int)f_array[a];
		//////////////////////////////////////////////////////////////////////////////////
		//Ignoring values to the right of the decimal point, so values are casted to ints/
		//////////////////////////////////////////////////////////////////////////////////
		if ((b&1) == 0 ) 
		{
			numEvens++;
		}
	}
	return numEvens;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : integer_evens
// Description  : Return the number of even ints in the array
//
// Inputs       : A reference to an int array.
//                The length of the array as an int
//
// Outputs      : an int for the number of even ints in the array
//
int integer_evens(int i_array[], int length)
{
	int a; //Counter variable
	int b; //Used to store value in i_array
	int numEvens = 0; //numEvens counter
	//If a number is odd, its rightmost bit is 1, so ANDing it with 1 checks the last bit
	for (a = 0; a<length; a++)
	{
		b = i_array[a];
		if ((b&1) == 0 )
		{
			numEvens++;
		}
	}
	return numEvens;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : count_bits
// Description  : Return the number of "1" bits in the binary representation of the given int
//
// Inputs       : An int
// Outputs      : int for the number of "1" bits in the binary representation of the given int
//
int count_bits (int a)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//https://stackoverflow.com/questions/15115646/turning-an-integer-to-its-binary-representation-using-c/
	//I used mrh and Barath Ravikumar's strategy for getting the binary representation of an int          /
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	int ones = 0;
	int c,k;
	/////////////////////////////////////////////////////////////////////////
 	// The loop moves through the binary string and uses the bitwise AND     /
 	// with 1 on each bit. If the bit is 1, 1 is returned, else 0 is returned/
 	// a counter variable, ones, counts how many 1s are in the binary string /
  	/////////////////////////////////////////////////////////////////////////
	for (c = 31; c >= 0; c--)
    	{
		//////////////////////////////////////
		//k stores a bit shifted version of a/
		//////////////////////////////////////
        	k = a >> c;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//The shifting changes the rightmost bit of the string, using the AND operation compares the rightmost bit to 1/
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        	if (k & 1)
		{
			//////////////////////////////////////////////////////////
			//If the rightmost bit is a 1, increment the number of 1s/
			//////////////////////////////////////////////////////////
			ones++;
		}
		
    	}
	return ones; 
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : integer_quicksort
// Description  : The function should sort the integer array from least to greatest using the quicksort method
//
// Inputs       : A reference to an int array
// 		  The left index of the array (the position to start sorting from)
// 		  The right index of the array (the position to sort to)
//
// Outputs      : void
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//I read about the algorithm on Wikipedia and used pseudocode from https://www.geeksforgeeks.org/quick-sort//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void integer_quicksort(int i_array[], int low, int high)
{
	if (low < high)
	{
		//pi is used to pivor to elements,
		int pi = partition(i_array, low, high);
		integer_quicksort(i_array, low, pi - 1);
		integer_quicksort(i_array, pi + 1, high);
	}
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : partition
// Description  : This function helps with the integer_quicksort function, by pivoting elements of the array
//		  
// Inputs	: An int  array
// 		  The left index of the array (the position to start sorting from)
// 		  The right index of the array (the position to sort to)
// Outputs      : An int used to pivot elements (index for swapping elements)
//
int partition(int i_array[], int low, int high)
{
	int i;
	int pivot = i_array[high];
	i = (low - 1);
	int j = 0;
	for (j = low; j <= high-1; j++)
	{
		if (i_array[j] <= pivot)
		{
			i++;
			////////////////////////////////////////////////
 			//Swap values in i_array[i+1] and i_array[high]/
 			///////////////////////////////////////////////////////////////////////////////////////////////////////
 			//The commented out code here works but it is hacky and I found it at                                 /
 			//https://graphics.stanford.edu/~seander/bithacks.html#SwappingValuesSubAdd,                          /
 			//The code I ended up using is from dor's formula on                                                  /
 			//https://stackoverflow.com/questions/1826159/swapping-two-variable-value-without-using-third-variable/
			//(((a) == (b)) || (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))));
			//(((i_array[j]) == (i_array[i])) || (((i_array[j]) ^= (i_array[i])), ((i_array[i]) ^= (i_array[j])), ((i_array[j]) ^= (i_array[i]))));
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//This method doesn't work when the values are equal and swapping them doesn't do anything anyway,/
			//so the code doesn't run if they're equal                                                        /
			///////////////////////////////////////////////////////////////////////////////////////////////////
			if (i_array[j] != i_array[i])
			{
				i_array[j]=i_array[j]+i_array[i];
				i_array[i]=i_array[j]-i_array[i];
				i_array[j]=i_array[j]-i_array[i];
			}
		}
	}
	////////////////////////////////////////////////
 	//Swap values in i_array[i+1] and i_array[high]/
 	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//The commented out code here works but it is hacky and I found it at                                 /
	//https://graphics.stanford.edu/~seander/bithacks.html#SwappingValuesSubAdd,                          /
	//the code I ended up using is from dor's formula on                                                  /
	//https://stackoverflow.com/questions/1826159/swapping-two-variable-value-without-using-third-variable/
	//(((i_array[i+1]) == (i_array[high])) || (((i_array[i+1]) ^= (i_array[high])), ((i_array[high]) ^= (i_array[i+1])), ((i_array[i+1]) ^= (i_array[high]))));
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//This method doesn't work when the values are equal and swapping them doesn't do anything anyway,/
	//so the code doesn't run if they're equal                                                        /
	///////////////////////////////////////////////////////////////////////////////////////////////////
	if (i_array[i+1] != i_array[high])
	{
		i_array[i+1]=i_array[i+1]+i_array[high];
		i_array[high]=i_array[i+1]-i_array[high];
		i_array[i+1]=i_array[i+1]-i_array[high];
	}
	return (i+1);
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : most_values
// Description  : This function should print the values which occur most in the array.
// 		  If multiple values occur the most, print the list of these values on a single line delimited by a single space
// 		  Megan told me that I could make this function void, the insutrctions say that it should return multiple values,
// 		  so I interpreted that as printing each of the values and not returning anything
//
// Inputs       : An int  array
//                The length of the int array as an int
//                The largest value that could appear in the array
//                
// Outputs      : void
//
//
void most_values(int i_array[], int length, int max)
{
	///////////////////////////////////////////////////////////////////////////////////
	//If the max value is n, the range of numbers will be -n to n and the +1 is from 0/
	//max is added to all the values in the array to account for negative values,     /
	//so -n will be in array location 0, +n will be in +2n                            /
	///////////////////////////////////////////////////////////////////////////////////
	int size = max*2+1;
	///////////////////////////////////////////////////////////////////////////////////
	//howManyTimes stores how many times each number appears, the default values are 0/
	//i is a counter variable                                                         /
	///////////////////////////////////////////////////////////////////////////////////
  	int howManyTimes[size];
 	int i;
	for (i = 0; i<size; i++)
	{
		howManyTimes[i]=0;
	}
	///////////////////////////////////////
	//b is used to store values in i_array/
	///////////////////////////////////////
  	int b;
  	for (i = 0; i<length; i++)
  	{
    		b=i_array[i];
    		howManyTimes[b+max]++;
  	}
	////////////////////////////////////////////////////////////////////
	//mostTimes used to store the largest value in howManyTimes        /
	//This value will be the largest amount of times any number appears/ 
  	////////////////////////////////////////////////////////////////////
	int mostTimes = 0;
  	for (i = 0; i<size; i++)
  	{
		////////////////////////////////////////////////////////
		//Finding the largest amount of times any value appears/
		////////////////////////////////////////////////////////
   		if (howManyTimes[i]>mostTimes)
    		{
     	 		mostTimes = howManyTimes[i];
    		}
  	}
  	printf("The highest amount of times one value appears is: ");
  	printf("%d",mostTimes);
  	printf("\n");
  	printf("These values appear ");
  	printf("%d",mostTimes);
  	printf(" times: ");
	///////////////////////////////////////////////////////
	//Finding all values that appear as often as mostTimes/
	//Print all the values that appear the most           /
	///////////////////////////////////////////////////////
  	for (i = 0; i<size; i++)
  	{
    		if (howManyTimes[i]==mostTimes)
    		{
     	 		printf("%d",i-max);
      			printf(" ");
    		}
  	}
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : reverse_bits
// Description  : This function returns the unsigned short int that results from reversing the bits
// 		  of the input unsigned short int
// 		  This function also prints the reversed binary and the resulting int from the reversed binary
//
// Inputs       : An unsigned short int
// Outputs      : The unsigned short int that results from reversing the bits of the input unsigned short int
//
unsigned short reverse_bits(unsigned short int rev)
{
	///////////////////////////////////////////////////////////////////////////
	//c is a counter variable, later used to sum the bits in the binary string/
	//k used to work with the bit shifting in order to get each bit of rev    /
	///////////////////////////////////////////////////////////////////////////
	int c, k;
	printf("REVR: ");
	/////////////////////////////////////////////////////////
	//Store the int in an array of ints represented in bits,/
	//unsigned short ints are 16 bits                       /
	/////////////////////////////////////////////////////////
	int bits[16];
	for (c = 0; c<=15; c++)
	{
		k = rev >> c;
		//////////////////////////////////////////////////////////////////////////
 		//If the bit ANDed with 1 returns 1, the bit must be 1, else it must be 0/
 		//////////////////////////////////////////////////////////////////////////
		if (k & 1)
		{
			printf("1");
 			bits[c]=1;
 		}
 		else
		{
			printf("0");
 			bits[c]=0;
 		}
 	}
	printf("\n");
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//Sum is used used to calculate the value of the binary string                                      /
	//b is used a s a counter variable for the array of ints                                            /
	//The max value of an unsigned short int is 32768, 2^15, the value of c divides until it reaches 2^0/
	/////////////////////////////////////////////////////////////////////////////////////////////////////
 	int sum = 0;
 	int b;
 	for (c = 32768, b=0; c>=1; c/=2,b++)
 	{
 		sum+=c*bits[b];
 	}
	printf("VAL: ");
	printf("%i", sum);
	//////////////////////////////////////////
	//Return the value of the reversed binary/
	//////////////////////////////////////////
 	return sum;  	
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : binary_string
// Description  : Populate the char array with the binary representation of the input unsigned int
//
// Inputs       : An unsigned short int
// 		  A reference to a char array
// 		  The length of the char array as an int
// Outputs      : void
//
void binary_string(unsigned short int a, char arr[], int length)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//This method for obtaining the binary representation of an int is the same as for count_bits         /
	//https://stackoverflow.com/questions/15115646/turning-an-integer-to-its-binary-representation-using-c/
	//I used mrh and Barath Ravikumar's strategy for getting the binary representation of an int          /
	//c is used as a counter variable for scanning through the array                                      /
	//k is used for bit shifting                                                                          /
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	int c, k;
	for (c = length-1; c>=0; c--)
	{
		//////////////////////////////////////
		//k stores a bit shifted version of a/
		//////////////////////////////////////
		k = a >> c;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//The shifting changes the rightmost bit of the string, using the AND operation compares the rightmost bit to 1/
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (k & 1)
		{
			////////////////////////////////////////////////////////
			//Print the current bit and add it to the array of bits/
			////////////////////////////////////////////////////////
			printf("1");
			arr[c]='1';
		}
		else
		{	
			////////////////////////////////
			//If not 1, print 0 and store 0/
			////////////////////////////////
			printf("0");
			arr[c]='0';
		}
	}
	///////////////////////////////
	//Adding the string terminator/
	///////////////////////////////
	arr[length-1]='\0';
}
