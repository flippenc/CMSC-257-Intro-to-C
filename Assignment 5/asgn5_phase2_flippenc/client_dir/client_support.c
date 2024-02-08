////////////////////////////////////////////////////////////////////////////////////
//
//  File          : client_support.c
//  Description   : This is the code for helper functions for the main file client.c
//
//  Author        : Christopher Flippen
//  Created       : 4/25/19
//

/////////////////
//Include files /
/////////////////
#include "client_support.h"

/////////////////////////////////////////////////////////////////
// Function     : get_in_addr
// Description  : Used to cast a sockaddr into a sockaddr_in pointer
//                the return value is used for inet_ntop
// Inputs       : A pointer to a sockaddr *sa
// Outputs      : void pointer to a sockaddr_in
//
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
