// A multithread echo server 

#include "mysocket.h"  
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 100

sem_t mutex;
int counter;

/* Structure of arguments to pass to client thread */
struct TArgs {
  TSocket cliSock;   /* socket descriptor for client */
};

/* Handle client request */
void * HandleRequest(void *args) {
	double n1[1], n2[1], ret[1];
	char sign;
	TSocket cliSock;
	int quit = 0;

	/* Extract socket file descriptor from argument */
	cliSock = ((struct TArgs *) args) -> cliSock;
	free(args);  /* deallocate memory for argument */

	while(!quit) {
		/* Receive the request */
		ReadDouble(cliSock, n1);
		printf("%.1lf ", *n1);
		ReadN(cliSock, &sign, 1);
		printf("%c ", sign);
		ReadDouble(cliSock, n2);
		printf("%.1lf = ", *n2);
	  
	 	if (sign == '+')
	 		*ret = *n1 + *n2;
	 	else if (sign == '-')
	 		*ret = *n1 - *n2;
	 	else if (sign == '*')
	 		*ret = (*n1) * (*n2);
	 	else if (sign == '/')
	 		*ret = *n1 / *n2;
	 	else if (sign == '?'){
	 		sem_wait(&mutex);
	 		*ret = counter;
	 		sem_post(&mutex);
	 	}
	 	else if (sign == '0'){
	 		sem_wait(&mutex);
	 		counter--;
	 		sem_post(&mutex);
	 		quit = 1;
	 		*ret = 0;
	 	}
	 	else
	 		*ret = 0;
		printf("%.1lf\n", *ret);
	  
		/* Send the response */
		WriteDouble(cliSock, ret);
	}
	close(cliSock);
	pthread_exit(NULL);
}

	TSocket srvSock, cliSock;        /* server and client sockets */
int main(int argc, char *argv[]) {
	struct TArgs *args;              /* argument structure for thread */
	pthread_t threads[NTHREADS];
	int tid = 0;
	int i = 0;
	
	sem_init(&mutex, 0, 1);
	counter = 0;
	
	if (argc == 1) { ExitWithError("Usage: server <local port>"); }

	/* Create a passive-mode listener endpoint */  
	srvSock = CreateServer(atoi(argv[1]));

	/* Run forever */
	for (;;) { 
		if (tid == NTHREADS) 
			ExitWithError("number of threads is over");

		/* Spawn off separate thread for each client */
		cliSock = AcceptConnection(srvSock);
		sem_wait(&mutex);
		counter++;
		sem_post(&mutex);
		printf("Thread %d criada.\n", ++i);

		/* Create separate memory for client argument */
		if ((args = (struct TArgs *) malloc(sizeof(struct TArgs))) == NULL)
			ExitWithError("malloc() failed");
		args->cliSock = cliSock;

		/* Create a new thread to handle the client requests */
		if (pthread_create(&threads[tid++], NULL, HandleRequest, (void *) args))
			ExitWithError("pthread_create() failed");
	}
    /* NOT REACHED */
	return 0;
}
