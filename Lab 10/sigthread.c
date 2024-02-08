#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

int counter = 0;

typedef struct data
{
	char name[10];
	int age;
} data;

void sig_handler(int sig) 
{
	write(1, "Caught signal 11\n", 17);
}

void SIGINT_handler(int sig)//,// int counter)
{
	counter++;
	printf("Caught signal SIGINT\n");
	if (counter >= 3)
	{
		printf("SIGINT received 3 times, killing");
		raise(SIGKILL);
	}
}

void SIGHUP_handler(int sig)
{
	printf("Caught signal SIGHUP\n");
}

void sig_func(data *p) 
{
	fprintf(stderr, "This is from thread function\n");
	strcpy(p->name,"Mr. Linux");
	p->age=30;
	//sleep(2);
}

int main() 
{
	int caught = 0;
	pthread_t tid;
	pthread_attr_t attr;
	data d;
	data *ptr = &d;
	// Register signal handler before going multithread
	
	struct sigaction new_action, old_action;
	new_action.sa_handler = SIGINT_handler;
	new_action.sa_flags = SA_NODEFER | SA_ONSTACK;
	sigaction(SIGINT, &new_action, &old_action);

	signal(SIGSEGV,sig_handler);
	//sigaction(SIGINT, SIGINT_handler);
	signal(SIGHUP, SIGHUP_handler);

	pthread_attr_init(&attr);
	pthread_create(&tid,&attr,(void*)sig_func,ptr);
	
	while(1)
	{
		select( 1, NULL, NULL, NULL, NULL );
	}
	
	//sleep(1); // Leave time for initialisation
	pthread_kill(tid,SIGSEGV);
	pthread_kill(tid,SIGINT);
	pthread_kill(tid,SIGHUP);

	pthread_join(tid,NULL);
 
	fprintf(stderr, "Name:%s\n",ptr->name);
	fprintf(stderr, "Age:%d\n",ptr->age);
}
