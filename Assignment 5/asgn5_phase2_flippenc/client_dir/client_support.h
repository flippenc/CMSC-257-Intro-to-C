#ifndef CLIENT_INCLUDED
#define CLIENT_INCLUDED

///////////////////////////////////////////////////////////////////////////
//
//  File          : client_support.h
//  Description   : This is the header file for client.c
//
//  Author        : Christopher Flippen
//  Created       : 4/22/19

/////////////////
//Include files /
/////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

/////////////////////////////////////////////////////////////
//Defines                                                   /
//ERROR and SUCCESS conditions make exit() more readable    /
//MAXDATASIZE used for defining size of the buffer to send  /
//and receive data with the server                          /
/////////////////////////////////////////////////////////////
#define ERROR_CONDITION 1
#define SUCCESS_CONDITION 0
#define MAXDATASIZE 50

///////////////////////////////////////
// Functions to define                /
// main is in client.c                /
// get_in_addr is in client_support.c /
///////////////////////////////////////
void *get_in_addr(struct sockaddr *sa);
int main(int argc, char **argv);

#endif
