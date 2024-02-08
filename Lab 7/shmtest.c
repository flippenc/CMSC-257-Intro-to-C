#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int *mem;

int main ( int argc, char **argv ) {
	int i, pid, kids, shmfd;
	//If no number given, default to 5
	//argc is number of children
	//Atoi converts string to int
	kids = argc > 1 ? atoi(argv[1]) : 5;
	
	//Open a file location /flippenc_memory for recording information about the network
	shmfd = shm_open ( "/flippenc_memory", O_RDWR | O_CREAT, 0666 );
	if ( shmfd < 0 )
	{
		fprintf(stderr,"Could not create brs_memory\n");
		exit(1);
	}
	//Truncate a file to a specified size, truncates file named path or referenced by pd to be truncated
	ftruncate ( shmfd, kids*sizeof(int) );
	
	//network mapper, checking what firewalls, OSs, etc. are being used on the server, checking available memory?
	mem = (int *)mmap ( NULL, kids*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	//If not able to find appropriate amount of memory, return and print a message
	if ( mem == NULL ) 
	{
		fprintf(stderr,"Could not map flippenc_memory\n");
		//Close processes, children, and returns
		exit(1);
	}
	//Close the file about network information
	close ( shmfd );
	//Delete the file which was storing information about the network
	shm_unlink ( "/flippenc_memory" );
	//Check how much memory each child process is using?
	for ( i = 0; i < kids; i++ ) 
	{
		mem[i] = i+1;
	}
	//Fork the processes and display information about them
	for ( i = 0; i < kids; i++ )
	{
		pid = fork();
		if ( pid < 0 ) 
		{
			//If fork fails
			fprintf(stderr,"fork failed at %d\n",i);
			exit(1);
		}
		else if ( pid > 0 ) 
		{
			printf("parent: new child is %d\n",pid);
		}
		else 
		{
			//If fork succeeds
			//Sleep for 1000 microseconds - 1 millisec
			usleep ( 1000 );
			printf("child %d, parent is %d\n",i, getppid());
			mem[i] = mem[i] * mem[i];
			exit(0);
		}
	}
	for ( i = 0; i < kids; i++ ) 
	{
		//wait until children processes finish
		wait(NULL);
	}
	for ( i = 0; i < kids; i++ ) 
	{
		//print out memory information
		printf("mem[%d] = %d\n",i,mem[i]);
	}
	return 0;
}
