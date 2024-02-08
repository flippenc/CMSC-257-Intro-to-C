/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "8512" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) 
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
	int sockfd, numbytes;
	//buffer length for transfering information is 100 bytes
	char buf[MAXDATASIZE];
	//addrinfo structs hints, servinfo, and p
	struct addrinfo hints, *servinfo, *p;
	int rv;
	//s is a string of size INET6_ADDRS
	char s[INET6_ADDRSTRLEN];

	//if incorrect command line parameters
	if (argc != 2) 
	{
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	//specifying hints' family and socktype values
	//specifying the memory location of hints
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	//Keep trying to socket for each addrinfo connected to servinfo
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) 
		{
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
		{
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}
	//If p went through all of the IP values and was still not able
	//to find valid data to socket with, 
	//print an error and return an error code
	if (p == NULL) 
	{
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	//converting the IP address from binary to text form so that it can print
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);
	//All the information about the server has been obtained
	freeaddrinfo(servinfo); // all done with this structure

	//If receiving data doesn't work, print an error and return an error code
	//Receives data from the server, it is stored into buf
	//recv(int sockfd, void *buf, size_t len, int flags)
	//MAXDATASIZE is the longest allowed message
	//recv returns the length of the message on successful completion
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) 
	{
	    perror("recv");
	    exit(1);
	}
	//end the string with a string terminator
	buf[numbytes] = '\0';

	//Client has received the data in the buffer
	printf("client: received '%s'\n",buf);
	//Finished with the socket
	close(sockfd);
	//Return, successful completion
	return 0;
}
