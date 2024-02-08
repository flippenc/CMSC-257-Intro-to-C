#include <sys/times.h>
#include<stdlib.h>
#include<stdio.h>

#define BSIZE ???

double start, stop, used, mf;

double ftime(void);
void multiply(double **a, double **b, double **c, int n);

double ftime (void)
{
	struct tms t;
	times(&t);
	return (t.tms_utime + t.tms_stime) / 100.0;
}

void multiply (double **a, double **b, double **c, int n)
{
	int i, j, k, jj, kk, sum;
	int en = BSIZE * (n/BSIZE);
   
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++)
			c[i][j] = 0;
	}

	for (kk=0; kk<en; kk+=BSIZE) {
		for (jj=0; jj<en; jj+=BSIZE) {
			for (i=0; i<n; i++) {
				for (j=jj; j <jj + BSIZE; j++) {
					sum = c[i][j];
					for (k=kk; k<kk + BSIZE; k++) {
						sum += a[i][k]*b[k][j];
					}
					c[i][j]=sum;
				}
			}
		}
	}
}

int main (void)
{
   int i, j, n;
   double **a, **b, **c;

    printf ( "Enter the value of n: ");
    scanf ( "%d", &n);

   //Populate arrays....
    a= (double**)malloc(n*sizeof(double));
    b= (double**)malloc(n*sizeof(double));
    c= (double**)malloc(n*sizeof(double));

	for (i=0; i<n; i++) {
		a[i]= malloc(sizeof(double)*n);
		b[i]= malloc(sizeof(double)*n);
		c[i]= malloc(sizeof(double)*n);
	}

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			a[i][j]=8;
			b[i][j]=7;
		}
	}

	start = ftime();
	multiply (a,b,c,n);
	stop = ftime();
	used = stop - start;
	mf = (n*n*n *2.0) / used / 1000000.0;
	printf ("\n");
	printf ( "Elapsed time:   %10.2f \n", used);
	printf ( "DP MFLOPS:       %10.2f \n", mf);
	return (0);
}