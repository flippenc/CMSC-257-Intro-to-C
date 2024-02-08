//////////////////////////////////////////////////////////////////////////////////////
//
//  File          : server_support.c
//  Description   : This is the code for helper functions for the main file server.c
//
//  Author        : Christopher Flippen
//  Created       : 4/25/19
//

/////////////////
//Include files /
/////////////////
#include "server_support.h"

///////////////////////////////////////////////////////////////////////////////
//
// Function     : childSIGUSR1
// Description  : Signal action for SIGUSR for the child part of server.c
//                Signal to notify the client that the server is closing,
//                close the open file, close the client socket, and exit
// Inputs       : signum - signal number
//                siginfo_t *info - used to obtain the file descriptor to close
//                void *ucontext - not used in this function
// Outputs      : void
//
void childSIGUSR1(int signum, siginfo_t *info, void *ucontext)
{
	printf("Received signal SIGUSR1 in child function\n");
	send(new_fd, "/KILLED", strlen("/KILLED"), 0);
	printf("Sent killed message to client\n");
	close(info->si_fd);
	printf("Closed file\n");
	close(new_fd);
	printf("Closed child socket\n");
	exit(1);
}

//////////////////////////////////////////////////////////////////////////////
//
// Function     : parentSIGINT
// Description  : Signal action for SIGINT for the parent part of server.c
//                Signal to kill all of the child
// Inputs       : int signum - signal number
//                siginfo_t *info - info not used in this function
//                void *ucontext - not used in this function
// Outputs      : void
//
void parentSIGINT(int signum, siginfo_t *info, void *ucontext)
{
	printf("Received signal SIGINT in parent function\n");
	close(sockfd);
	int i = 0;
	for (i = 0 ; i < MAXKIDS ; i++)
	{
		kill(kidsPID[i], SIGUSR1);
		wait(NULL);
	}
	exit(1);
}

////////////////////////////////////////////////////////////////////////////////////////
//
// Function     : parentSIGHLD
// Description  : Signal action for SIGCHLD for the parent part of server.c
//                Closes the desired child by looping through the array of
//                PIDs and closing the one specified in info
// Inputs       : int signum - signal number
//                siginfo_t *info - info used for knowing which child to close
//                void *ucontext - not used, but needed to fit the format of sigaction
// Outputs      : void
//
void parentSIGCHLD(int signum, siginfo_t *info, void *ucontext)
{
	int PID = info -> si_pid;
	int i = 0;
	for (i = 0; i < MAXKIDS; i++)
	{
		if (kidsPID[i] == PID)
		{
			waitpid(PID, NULL, 0);
			kidsPID[i] = 0;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// Function     : child
// Description  : Used for taking care of the client
//                This function gets a filename from the client, opens the file
//                and sends its contents to the client, size bytes at a time
//                Size isn't necessarily needed since the main function uses
//                MAXDATASIZE as the parameter for size
// Inputs       : An int for the client socket and an int for the size of the
//                buffer
// Outputs      : void
//
void child(int client, int size)
{
	struct sigaction newact, oldact;	

	////////////////////////////////////
	//Specifying sigaction for SIGINT  /
	////////////////////////////////////
	newact.sa_handler = SIG_IGN;
	sigaction(SIGINT, &newact, &oldact);

	////////////////////////////////////
	//Specifying sigaction for SIGCHLD /
	////////////////////////////////////
	newact.sa_handler = SIG_DFL;
	newact.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &newact, &oldact);
	
	////////////////////////////////////
	//Specifying sigaction for SIGUSR  /
	////////////////////////////////////
	newact.sa_handler = NULL;
	newact.sa_sigaction = childSIGUSR1;
	newact.sa_flags = SA_NOCLDWAIT | SA_SIGINFO;
	sigaction(SIGUSR1, &newact, &oldact);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Strings used for sending messages to the client                                                               /
	//endString tells the client the whole file has been sent or the file does not exist                            /
	//killString tells the client that the server process has been killed, so no more of the file will be sent over /
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	char endString[] = "/CMSC257";
	char killString[] = "/KILLED";

	/////////////////////////////////////////////////////////////////////////////////////////
	//File is the file descriptor for the file that will be sent to the client              /
	//i is used as a counter variables in loops                                             /
	//buf is used to send the file and other messages to the client                         /
	//FILENAME is string containing the file name, this will be used to parse the filename  /
	//FILENAME has trouble parsing file names unless it has a large array size              /
	//n is used to keep track of return values from some functions                          /
	/////////////////////////////////////////////////////////////////////////////////////////
	int file;
	int i = 0;
	char buf[size];
	char FILENAME[10000];
	int n = size;
	
	///////////////////////////////////////////////////////////////////////////
	//Read the name of the requested file from the client and print a message /
	///////////////////////////////////////////////////////////////////////////
	n = read(client, FILENAME, size);
	if (n < 0)
        {
                printf("Error reading filename from client, so sending end of file string\n");
                send(client, endString, strlen(endString),0);
                exit(ERROR_CONDITION);
        }
	//////////////////////////////////////////////////////////////////////////
	//realFile used to obtain the name of the file from FILENAME,            /
	//the pointer is increased by 4 to get from GET nameOfFile to nameOfFile /
	//////////////////////////////////////////////////////////////////////////
        char *realFile = FILENAME+4;
        printf("Filename is %s\n", realFile);
        
	/////////////////////////////////////////////////////////////
	//Try to open the file                                      /
        //If the file does not open correctly,                      /
        //send a message to the client and close the child function /
        /////////////////////////////////////////////////////////////
        file = open(realFile, O_RDWR, S_IRUSR | S_IWUSR);
        if ( file < 0 )
        {
        	printf("Error opening file, so sending end of file string\n");
        	send(client, endString, strlen(endString),0);
        	exit(ERROR_CONDITION);
        }
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//If the file opened correctly, send it to the client with buf                                    /
	//Add NULL chars to the end of the buffer in case the data being sent is <50 bytes,               /
	//this prevents garbage or repeat data from being sent                                            /
	//If the child is killed while sending data, exit the loop and move down to the next if statement /
	///////////////////////////////////////////////////////////////////////////////////////////////////
	printf("File opened correctly\n");
	n = size;
	while(n == size)
	{
		memset(buf, 0, MAXDATASIZE);
		n = read(file, buf, n);
		//Print statement used for debugging server code
		//printf("Current buf contains: %s\n",buf);
		send(client,buf,n,0);
	}
	////////////////////////////////////////////////////////////////
	//If the program gets here, then the file has been fully sent, /
	//so it's time to send the end of file message                 /
	////////////////////////////////////////////////////////////////
	close(file);
	send(client, endString, strlen(endString), 0);
	printf("Sent end of file message\n");
	exit(SUCCESS_CONDITION);
}

///////////////////////////////////////////////////////////////
//
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
