// Include Files
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Project Includes
#include "lab3header.h"

// Function Implementations

int reverse_number(int num)
{
	while (num > 0)
	{
		printf("%d",num%10); num = num/10;
	}
	printf("\n");
	return num;
}


int reverse_number_do_while(int num)
{
	do {
		printf("%d",num%10); num = num/10;
	} while (num > 0);
	printf("\n");
	return num;

}

int even_odd(int num)
{
	if (num%2 == 0)
	{
		printf("Even\n");
		return 0;
	}
	else printf("Odd\n");
	return 1;
}


int prime(int num)
{
	int flag = 0;
	int i = -1;
	for (i=2;i<=num/2;i++)
	{
		if (num%i == 0 )
		{
			flag = 1;
			break;
		}
	}
		if (flag == 0)
		{
			printf("Number is prime\n");
		}
		else
		{	
			printf("Number is not prime\n");
		}
	return 1;
}
