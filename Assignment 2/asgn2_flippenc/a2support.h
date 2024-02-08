#ifndef A2SUPPORT_INCLUDED
#define A2SUPPORT_INCLUDED

//////////////////////////////////////////////////////////////////////////////
//
//  File          : a2support.h
//  Description   : This is the header file for the functions for assignment
//                  1 of the CMSC257 course.  Students are required to define
//                  the following functions and implement them in another
//                  file, a1support.c.
//
//  Author        : Christopher Flippen
//  Created       : 2/20/19

// Functions to define

void float_display_array(float f_array[], int length);
void integer_display_array(int i_array[], int length);
int float_evens(float f_array[], int length);
int integer_evens(int f_array[], int length);
int count_bits(int a);
void integer_quicksort(int i_array[], int low, int high);
int partition(int i_array[], int low, int high);
void most_values(int i_array[], int length, int max);
unsigned short reverse_bits(unsigned short int rev);
void binary_string(unsigned short int a, char arr[], int length);

#endif

