/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "8500"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) 
{
	if (sa->sa_family == AF_INET) 
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	//If getting address info fails, print an error and return an error code
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		//obtain the first socket that works, if it fails, print an error
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) 
		{
			perror("server: socket");
			continue;
		}
		//if setting the options for the socket fails,
		//print an error message and exit with an error code
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) 
		{
			perror("setsockopt");
			exit(1);
		}

		//bind the socket, if it fails, close it and continue the loop
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}
	//the information about the server has been obtained and used, this structure is no longer needed
	freeaddrinfo(servinfo); // all done with this structure

	//if p is null, then the socket was not correctly bound
	if (p == NULL)  
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	//making the socket listen, if it fails, then print an error and exit with error code 1
	if (listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}

	//If the program gets here, the socket is working and waiting
	printf("server: waiting for connections...\n");

	//Loop while waiting for data
	while(1) 
	{  // main accept() loop
		sin_size = sizeof their_addr;
		//accept the socket with the client's address
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) 
		{
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);
		//Try to send a "Hello, world" message when the connection has been made
		//If the connection fails here, print an error,
		if (send(new_fd, "Hello, world!", 13, 0) == -1)
		{
			perror("send");
		}
		close(new_fd);
	}
	return 0;
}
