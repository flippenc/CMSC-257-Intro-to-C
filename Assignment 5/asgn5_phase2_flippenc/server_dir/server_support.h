#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

//////////////////////////////////////////////////////////////////////////////
//
//  File          : server_support.h
//  Description   : This is the header file for server.c and server_support.c
//
//  Author        : Christopher Flippen
//  Created       : 4/22/19

///////////////////////////////////////////////////////////////////////////////////////////////
//Defines                                                                                     /
//ERROR and SUCCESS conditions make exit() more readable                                      /
//BACKLOG used for the listen function                                                        /
//MAXDATASIZE used for defining size of the buffer to send data to the client                 /
//MAXKIDS specifies the maximum number of clients that can be connected to the server at once /
///////////////////////////////////////////////////////////////////////////////////////////////
#define ERROR_CONDITION 1
#define SUCCESS_CONDITION 0
#define MAXDATASIZE 50
#define BACKLOG 10
#define MAXKIDS 5

/////////////////////////////////////////////////////////////////////////////////
//Global variables                                                              /
//kids is used to keep track of number of child processes                       /
//kidsPID holds the PIDs of all current child processes                         /
//sockfd and new_fd are the server and client sockets respectively              /
/////////////////////////////////////////////////////////////////////////////////
int kids;
int kidsPID[MAXKIDS];
int sockfd;
int new_fd;

////////////////////////
// Functions to define /
////////////////////////
void main(int argc, char **argv);
void childSIGUSR(int signum, siginfo_t *info, void *ucontext);
void parentSIGINT(int signum, siginfo_t *info, void *ucontext);
void parentSIGCHLD(int signum, siginfo_t *info, void *ucontext);
void child(int client, int size);
void *get_in_addr(struct sockaddr *sa);

#endif
