#include <sys/mman.h>
#include <sys/times.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#define BSIZE ???
TODO Change names of shared memory segments and semaphore to include your eID

double start, stop, used, mf;
sem_t *sem;

double ftime(void);
void multiply (double **a, double **b, int n);

double ftime (void) {
    struct tms t;
    times(&t); 
    return (t.tms_utime + t.tms_stime) / 100.0;
}

void multiply (double **a, double **b, int n) {
    int i, j, k, kk, jj, pid, kids=0, shmfd;
    int en = BSIZE * (n/BSIZE);
	int arrLen = (n*n*sizeof(double));
	double *memA, *memB, *memC;
	
	shmfd = shm_open("/eid_a_memory", O_RDWR | O_CREAT, 0666);
	if(shmfd < 0) {
		fprintf(stderr, "Could not create a_memory\n");
		exit(1);
	}
	if(ftruncate(shmfd, arrLen) == -1) {
		fprintf(stderr,"Error truncating a_memory\n");
		exit(1);
	}
	memA = (double *) mmap(NULL, arrLen, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if(memA == ((void *) -1)) {
		fprintf(stderr,"Could not map a_memory\n");
		exit(1);
	}
	close(shmfd);
	shm_unlink("/eid_a_memory");
	
	shmfd = shm_open("/eid_b_memory", O_RDWR | O_CREAT, 0666);
	if(shmfd < 0) {
		fprintf(stderr, "Could not create b_memory\n");
		exit(1);
	}
	if(ftruncate(shmfd, arrLen) == -1) {
		fprintf(stderr,"Error truncating b_memory\n");
		exit(1);
	}
	memB = (double *) mmap(NULL, arrLen, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if(memB == ((void *) -1)) {
		fprintf(stderr,"Could not map b_memory\n");
		exit(1);
	}
	close(shmfd);
	shm_unlink("/eid_b_memory");
	
	shmfd = shm_open("/eid_c_memory", O_RDWR | O_CREAT, 0666);
	if(shmfd < 0) {
		fprintf(stderr, "Could not create c_memory\n");
		exit(1);
	}
	if(ftruncate(shmfd, arrLen) == -1) {
		fprintf(stderr,"Error truncating c_memory\n");
		exit(1);
	}
	memC = (double *) mmap(NULL, arrLen, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if(memC == ((void *) -1)) {
		fprintf(stderr, "Could not map c_memory\n");
		exit(1);
	}
	close(shmfd);
	shm_unlink("/eid_c_memory");
	
	shmfd = shm_open("/eid_sem_memory", O_RDWR | O_CREAT, 0666);
	if(shmfd < 0) {
		fprintf(stderr, "Could not create a_memory\n");
		exit(1);
	}
	if(ftruncate(shmfd, sizeof(sem_t)) == -1) {
		fprintf(stderr,"Error truncating a_memory\n");
		exit(1);
	}
	sem = sem_open("eid_c_sem", O_CREAT, 0666, 1);
	if(sem == NULL) {
		fprintf(stderr,"Could not create c semaphore\n");
		exit(1);
	}
	sem_unlink("eid_c_sem");
	close(shmfd);
	shm_unlink("/eid_sem_memory");
	
	for(i=0;i<n;i++) {
		for(j=0;j<n;j++) {
			memA[i*n+j] = a[i][j];
			memB[i*n+j] = b[i][j];
			memC[i*n+j] = 0;
		}
	}

    for(kk=0; kk<en; kk+=BSIZE) {
        for(jj=0; jj<en; jj+=BSIZE) {
				kids++;
				pid = fork();
				if(pid<0) {
					fprintf(stderr, "fork failed at %d\n", jj);
					exit(1);
				} else if(pid == 0) {
					int sum;
                    	for(i=0; i<n; i++) {
                            for(j=jj; j<jj+BSIZE; j++) {
                                sum = memC[i*n+j];
                                for(k=kk; k<kk+BSIZE; k++) {
                                    sum += memA[i*n+k] * memB[k*n+j];
                                }
								sem_wait(sem);
                                memC[i*n+j]=sum;
								sem_post(sem);
                            }
                        }
					exit(0);
				}
        }
    }

	for(i=0; i<kids; i++) {
		wait(NULL);
	}
}

int main (void) {
    int i, j, n;
    double **a, **b;

    printf ( "Enter the value of n: ");
    scanf ( "%d", &n);

   //Populate arrays....
    a= (double**)malloc(n*sizeof(double));
    b= (double**)malloc(n*sizeof(double));

	for (i=0; i<n; i++) {
		a[i]= malloc(sizeof(double)*n);
		b[i]= malloc(sizeof(double)*n);
	}

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			a[i][j]=8;
			b[i][j]=7;
		}
	}

	start = ftime();
	multiply (a,b,n);
	stop = ftime();
	used = stop - start;
	mf = (n*n*n *2.0) / used / 1000000.0;
	printf ("\n");
	printf ( "Elapsed time:   %10.2f \n", used);
	printf ( "DP MFLOPS:       %10.2f \n", mf);
	return (0);
}
