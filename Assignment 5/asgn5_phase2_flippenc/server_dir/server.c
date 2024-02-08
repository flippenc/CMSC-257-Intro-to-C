/////////////////////////////////////////////////////////////////////////////////////
//
//  File          : server.c
//  Description   : This is the main method for the server, this file uses functions
//  		    from server_support.c to create the server functionality
//
//  Author        : Christopher Flippen
//  Created       : 4/21/19
//

/////////////////
//Include files /
/////////////////
#include "server_support.h"

/////////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : Main function, obtains port from command line parameters
//
// Inputs       : argc is number of parameters, argv are the command line arguments
// Outputs      : int - error codes
//
void main(int argc, char **argv)
{
	kids = 0;
	/////////////////////////////////////////////////////////////////////////////////////////
	//Specifying what the parent function will do if it receives a SIGINT or SIGCHLD signal /
	//////////////////////////////////////////////////////////////////////////////
	struct sigaction newact, oldact;
	newact.sa_sigaction = parentSIGCHLD;
	newact.sa_flags = SA_NOCLDWAIT | SA_SIGINFO;
	sigaction(SIGCHLD, &newact, &oldact);	

	newact.sa_sigaction = parentSIGINT;
	newact.sa_flags = SA_NOCLDWAIT | SA_SIGINFO;
	sigaction(SIGINT, &newact, &oldact);

	////////////////////////////////////////////////////////////////////////////////
	//If the number of command line parameters != 2, then print a message and exit /
	////////////////////////////////////////////////////////////////////////////////
	if (argc != 2)
	{
		printf("Error in form of server parameters\n");
		printf("Format should be ./server [PORT]\n");
		exit(ERROR_CONDITION);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//If there is the correct number of parameters, check that the supplied port number is valid /
	//////////////////////////////////////////////////////////////////////////////////////////////
	int checkPort = strtol(argv[1], NULL, 10);
	printf("Port is %i\n", checkPort);

	///////////////////////////////////////////
	//Ports should be between 50000 and 60000 /
	///////////////////////////////////////////
	if ( checkPort < 50000 || checkPort > 60000 )
	{
		printf("Error in PORT number\n");
		printf("Port number must be between 50000 and 60000\n");
		exit(ERROR_CONDITION);
	}

	///////////////////////////////////////////////////////////
	//Variables for connecting to the client                  /
	//sockfd is the server socket                             /
	//new_fd is the client socket                             /
	//hints, servinfo, and p store information for connecting /
	//sin_size used for accept                                /
	//yes is used as a parameter for setsockopt		  /
	//s is used in the parameters in inet_ntop                /
	//rv stores rturn value of getaddrinfo                    /
	//i is used as a counter variable                         /
	///////////////////////////////////////////////////////////
	//sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int yes=1;
	char s[INET_ADDRSTRLEN];
	int rv;
	int i = 0;

	//////////////////////////////////////////////////////////////
	//Hints stores information used for connecting to the client /
	//////////////////////////////////////////////////////////////
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	////////////////////////////////////////////////////////////////
	//Use getaddrinfo to get information needed to connect,        /
	//print a message and exit with an error condition if it fails /
	////////////////////////////////////////////////////////////////
	if (( rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0)
	{
		printf("Error in getaddrinfo\n");
		exit(ERROR_CONDITION);
	}

	/////////////////////////////////////////////////////////
	//Creating the socket, setting its options,             /   
	//and binding it using information in servinfo          /
	//Loop through the values in servinfo to try to connect /
	//If setting the server options returns an error,       /
	//something is wrong and the server closes              /
	/////////////////////////////////////////////////////////
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			printf("Error in creating socket\n");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			close(sockfd);
			printf("Error in setting socket options\n");
			exit(ERROR_CONDITION);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1 )
		{
			printf("Error in binding server\n");
			continue;
		}
		break;
	}
	////////////////////////////////////////////////////
	//servinfo is no longer needed, so it can be freed /
	////////////////////////////////////////////////////
	freeaddrinfo(servinfo);
	
	///////////////////////////////////////////////////////////
	//Checking if binding and socket creation were successful /
	///////////////////////////////////////////////////////////
	if (p == NULL)
	{
		printf("Error in binding server\n");
		exit(ERROR_CONDITION);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//listen marks socket as being a passive socket - used to accept incoming connection requests /
	///////////////////////////////////////////////////////////////////////////////////////////////
	if (listen(sockfd, BACKLOG) == -1)
	{
		printf("Error listening\n");
		exit(ERROR_CONDITION);
	}
	
	//////////////////////////////////////////
	//The server can now receive connections /
	//////////////////////////////////////////
	printf("Server: waiting for connections...\n");

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Keep looping while connections are being made, the loop only ends if it receives a signal /
	/////////////////////////////////////////////////////////////////////////////////////////////
	while(1)
	{
		sin_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

		//////////////////////////////////////////////////////
		//If accepting the connection fails, print a message /
		//////////////////////////////////////////////////////
		if (new_fd == -1)
		{
			printf("Error accepting\n");
			continue;
		}
		////////////////////////////////////////////////////////////////////
		//Convert the IP into a form that can be printed and               /
		//print a message that a connection has been received successfully /
		////////////////////////////////////////////////////////////////////
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),s, sizeof(s));
		printf("Server: got a connection from %s\n", s);
	
		///////////////////////////////////////////////////////////////////////
		//If a client tries to connect when the server has too many clients,  /
		//then the server sends a message to the client, telling it to close, /
		//else connect normally with forking and take care of the client      /
		///////////////////////////////////////////////////////////////////////
		if ( kids >= 5)
		{
			send(new_fd,"/SERVER FULL", strlen("/SERVER FULL"),0);
			close(new_fd);

		}
		else
		{
			///////////////////////////
			//PID is used for forking /
			///////////////////////////
			int PID;

			////////////////////////////////////////////////////////////////////////////////////
			//Fork, PID < 0 means an error occured, PID == 0 means child, PID > 0 means parent /
			////////////////////////////////////////////////////////////////////////////////////
			PID = fork();
			if (PID < 0 )
			{
				printf("Error in PID\n");

			}
			//////////////////////////////////////////////////////
			//Child function takes care of the client connection /
			//////////////////////////////////////////////////////
			else if (PID == 0)
			{
				////////////////////////////////////////////////////////////
				//Once the child function is finished, exit the child      /
				//Kids keeps track of how many child processes are running /
				////////////////////////////////////////////////////////////
				child(new_fd, MAXDATASIZE);
				exit(0);
			}
			////////////////////////////////////////////////////////////////////////////////////
			//Parent function takes care of closing the child function                         /
			//Store the PID of the current child in the array of PIDs and increments kids      /
			//When the child process is over, remove the PID from the array and decrement kids /
			////////////////////////////////////////////////////////////////////////////////////
			else
			{
				kids++;
				for (i = 0; i < MAXKIDS; i++)
				{
					if (kidsPID[i] != 0)
					{
						kidsPID[i] = PID;
						break;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////
				//Wait for the child to finish, close it, decrement kids and remove PID from pool /
				///////////////////////////////////////////////////////////////////////////////////
				wait(NULL);
				kids--;
				for (i = 0; i <MAXKIDS; i++)
				{
					if (kidsPID[i] == PID)
					{
						kidsPID[i] = 0;
						break;
					}
				}
       				close(new_fd);
			}
		}
	}
}
