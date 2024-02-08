#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
//10000 sizeof(int) worth of memory per child, each process runs for 1 millisecond
//2D array, [0][0], [1][1], [2][2], [3][3], [4][4], each stores (n+1)^2
int (*mem);

//Push the parent functionality into a function: parent_fun()
//Just call this function from the parent part of the code
void parent_fun(int pid)
{
	printf("parent: new child is %d\n",pid);
}

void child_fun(int i, int n)
{
	usleep ( 1000 );
	printf("child %d, parent is %d\n",i, getppid());
	mem[i*n+i] = mem[i*n+i] * mem[i*n+i];
	exit(mem[i*n+i]);
}

int main ( int argc, char **argv ) 
{
	int i, pid, kids, shmfd;
	//Default 5 kids
	kids = argc > 1 ? atoi(argv[1]) : 5;
	//Default value of n
	int n = argc > 1 ? atoi(argv[1]) : 5;
	//Default columns 10000
	int columns = argc > 2 ? atoi(argv[2]) : 10000;
	
	//Creating page "/flippenc_memory"
	shmfd = shm_open ( "/flippenc_memory1", O_RDWR | O_CREAT, 0666 );
	if ( shmfd < 0 )
	{
		fprintf(stderr,"Could not create flippenc_memory\n");
		exit(1);
	}
	
	//Truncate the page to kids*columns*sizeof(int);
	ftruncate ( shmfd, kids*columns*sizeof(int));
	
	//Allocating n^2*columns*sizeof(int)
	mem = (int (*))mmap ( NULL, (kids*columns)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	if ( mem == NULL ) 
	{
		fprintf(stderr,"Could not map flippenc_memory\n");
		exit(1);
	}
	
	//Closing and unlinking page "/flippenc_memory"
	close ( shmfd );
	shm_unlink ( "/flippenc_memory1" );

	for ( i = 0; i < kids; i++ ) 
	{
		mem[i*n+i] = i+1;
	}
	
	for ( i = 0; i < kids; i++ ) 
	{
		//child_fun(i,n);
        	pid = fork();
        	if ( pid < 0 )
        	{
                	fprintf(stderr,"fork failed at %d\n",i);
                	exit(1);
        	}
        	else if ( pid > 0 )
        	{
                	parent_fun(pid);
        	}
        	else
        	{
			child_fun(i,n);
		}

		int exitStatus;
		wait(&exitStatus);
		if (exitStatus > 0)
		{
			printf("Child exited with %d\n",WEXITSTATUS(exitStatus));
		}
	}
	for ( i = 0; i < kids; i++ ) 
	{	
		printf("mem[%d] = %d\n",i*n+i,mem[i*n+i]);
	}
	return 0;
}
