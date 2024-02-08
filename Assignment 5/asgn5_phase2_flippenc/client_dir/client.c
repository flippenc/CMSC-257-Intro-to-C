////////////////////////////////////////////////////////////////////////////
//
//  File          : client.c
//  Description   : This is the main function for the client code
//
//  Author        : Christopher Flippen
//  Created       : 4/21/19
//

/////////////////
//Include files /
/////////////////
#include "client_support.h"

////////////////////////////////////////////////////////////////
// 
// Function	: main
// Description	: Main function, obtains command line parameters, 
//		  connects to the server, sends a request for a file, 
//		  and receives it 50 bytes at a time
// Inputs	: argc is number of parameters, argv are the command line arguments
// Outputs	: int - error codes
//
int main(int argc, char **argv)
{
	//////////////////////////////////////////////////////////////////////////
	//sockfd used for the socket                                             /
	//buf used for sending and receiving messages                            /
	//hints, servinfo, and p store information used for connecting           /
	//rv is used for checking return values                                  /
	//s is used for storing the IP address when its converted with inet_ntop /
	//////////////////////////////////////////////////////////////////////////
	int sockfd;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	/////////////////////////////////////////////////////////////
	//Checking that number of command line arguments is 4       /
	//If the program receives a different number of arguments,  /
	//it won't have the information it needs to run the program /
	/////////////////////////////////////////////////////////////
	if ( argc != 4 )
        {
                printf("Error in form of client parameters\n");
                printf("Format should be ./client [IP] [PORT] [FILENAME]\n");
                exit(ERROR_CONDITION);
        }
	/////////////////////////////////////////////////////////////////////////////////////
	//If the program has the right number of arguments, check that the port is valid    /
	//Convert the port number from the arguments into an int and store it in a variable /
	/////////////////////////////////////////////////////////////////////////////////////
        int PORT = strtol(argv[2], NULL, 10);
        printf("Port is %i\n",PORT);

	///////////////////////////////////////////
	//The port must be between 50000 and 60000/
	///////////////////////////////////////////
        if ( PORT < 50000 || PORT > 60000 )
        {
                printf("Error in PORT number\n");
                printf("Port number must be between 50000 and 60000\n");
	}
	
	//////////////////////////////////////////////////////////////////////
	//Check that the filename is less than 45 bytes long                 / 
	//The filename mustbe able to fit in the 50 byte buffer with         /
	//the added null terminator and the "GET " string concatenated to it /
	//////////////////////////////////////////////////////////////////////
	if ( strlen(argv[3]) > MAXDATASIZE-5)
        {
                printf("Error in filename\n");
                printf("Filename must be less than %i bytes\n",MAXDATASIZE-5);
                exit(ERROR_CONDITION);
        }

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Store the parameter into a variable called FILENAME and add a string terminator char to it /
	//////////////////////////////////////////////////////////////////////////////////////////////
	char FILENAME[MAXDATASIZE];
	strcpy(FILENAME, argv[3]);
	FILENAME[strlen(argv[3])] = '\0';
	printf("Filename is %s\n", FILENAME);
	
	/////////////////////////////////////////////////////////////////////////////////
	//If the user uses localhost as the IP, convert the argument into a loopback IP /
	/////////////////////////////////////////////////////////////////////////////////
	if (!strcmp(argv[1], "localhost"))
	{
		argv[1] = "127.0.0.1";
	}
        printf("IP is %s\n",argv[1]);
	
	////////////////////////////////////////////////////////
	//hints stores information used to make the connection /
	////////////////////////////////////////////////////////
	memset(&hints, 0 , sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/////////////////////////////////////////////////////////////////////////////////
	//Get information about the server, used for creating the socket and connecting /
	/////////////////////////////////////////////////////////////////////////////////
	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0)
	{
		printf("Error in getaddrinfo\n");
		exit(ERROR_CONDITION);
	}
	
	//////////////////////////////////////////////////////////////////
	//Create the socket, and connect using information from servinfo /
	//////////////////////////////////////////////////////////////////
	for(p = servinfo; p!= NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			printf("Error in creating socket\n");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			printf("Error in client connecting\n");
			continue;
		}
		break;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//If the socket was creating incorrectly or the connection could not be made, /
	//print an error and return                                                   /
	///////////////////////////////////////////////////////////////////////////////
	if (p == NULL)
	{
		printf("Error connecting\n");
		exit(ERROR_CONDITION);
	}

	////////////////////////////////////////////////////////////////////
	//Convert the IP address into a form that can be printed and       /
	//display a message saying that the client has connected correctly /
	////////////////////////////////////////////////////////////////////
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
	printf("Client connecting to %s\n", s);

	char getFILENAME[50];
	strcat(getFILENAME,"GET ");
	strcat(getFILENAME,FILENAME);
	///////////////////////////////////////////////////////////////////////////////////
	//Send a request to the server including the desired file,                        /
	//if the send fails, print a message and close the client with an error condition /
	///////////////////////////////////////////////////////////////////////////////////
	if (send(sockfd, getFILENAME, strlen(getFILENAME), 0) < 0)
        {
                printf("Error sending message\n");
                exit(ERROR_CONDITION);
        }
        printf("Sent message to server, %s\n",getFILENAME);	

	//////////////////////////////////////////////////////////////
	//file is a file descriptor                                  /
	//clientFile is the name of the file that will be written to /
	//////////////////////////////////////////////////////////////
	int file;
	//char clientFile[50] = "received.txt";
	//Open the file, print and message and close the client if the file cannot be opened
	file = open(FILENAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (file < 0 )
	{
		printf("Error opening file\n");
		exit(ERROR_CONDITION);
	}
	printf("File %s opened correctly\n",FILENAME);	
	/////////////////////////////////////////////////////////////////////////////
	//i and n used for storing information while receiving data from the server /
	/////////////////////////////////////////////////////////////////////////////
	int i = 0;
	int n;
	printf("Reading data from server\n");
	
	///////////////////////////////////////////////////////////////////////////
	//Loop until all data has been read or the client receives a kill message /
	///////////////////////////////////////////////////////////////////////////
	while (1)
	{
		memset(buf, 0, MAXDATASIZE);
		///////////////////////////
		//Read 50 bytes at a time /
		///////////////////////////
		n = read(sockfd, buf, MAXDATASIZE);
		if (n < 0)
		{
			printf("Error reading data\n");
			close(file);
			exit(ERROR_CONDITION);
		}
		/////////////////////////////////////////////////////////////////////////////
		//If the received message is the end of file message, break out of the loop /
		/////////////////////////////////////////////////////////////////////////////
		if (strstr(buf, "/CMSC257") != NULL)
		{
			write(file, buf, n-8);
			printf("\nCurrent buffer contains %s\n",buf);
			printf("\nClient received /CMSC257, so file is finished\n");
			break;
		}
		/////////////////////////////////////////////////////////////////////
		//If the received message is the kill string, break out of the loop /
		/////////////////////////////////////////////////////////////////////
		if (strstr(buf, "/KILLED") != NULL)
		{
			printf("\nCurrent buffer contains %s\n",buf);
			printf("\nClient received /KILLED, so the server connection has been killed\n");
			printf("\nThe full file has not been received, so the incomplete file will be deleted\n");
			unlink(FILENAME);
			break;
		}
		if (strstr(buf, "/SERVER FULL") != NULL)
		{
			printf("\nCurrent buffer contains %s\n",buf);
			printf("\nClient received /SERVER FULL, so the server has too many clients currently\n");
			printf("\nTry to connect later\n");
			unlink(FILENAME);
			break;
		}
		//printf("buf is %s\n",buf);
		//////////////////////////////////////////////////
		//Write from buf to the file, 50 bytes at a time /
		//////////////////////////////////////////////////
		if (write(file, buf, n) < 0 )
		{
			printf("Error writing to file\n");
			exit(ERROR_CONDITION);
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Empty the buffer after each write to prevent garbage being written or data being written multiple times /
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//for (i = 0; i<MAXDATASIZE; i++)
                //{
                 //       buf[i] = '\0';
                //}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	//If the program gets here, it has either read the whole file or gotten a kill signal, /
	//so it is time to close everything                                                    /
	////////////////////////////////////////////////////////////////////////////////////////
	close(file);
	close(sockfd);
	freeaddrinfo(servinfo);
	printf("Closing program\n");
}
