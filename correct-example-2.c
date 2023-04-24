/**
 * A simple pthread program illustrating RT pthread scheduling.
 *
 * To compile:
 *
 *	gcc thrd-sched.c -o thrd-sched -lpthread
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Eighth Edition
 * Copyright John Wiley & Sons - 2009.
 */

#include <pthread.h>	/* for pthread functions */
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5

/* the thread runs in this function */
void *runner(void *param); 

main(int argc, char *argv[])
{
	int i;
	int localtid[NUM_THREADS];
	pthread_t tid[NUM_THREADS]; 	/* the thread identifier */
	pthread_attr_t attr; 		/* set of attributes for the thread */

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the threads */
	printf("Main: Parent thread creating child threads\n");
	for (i = 0; i < NUM_THREADS; i++) { 
		localtid[i-1] = i;
		/* Note that we are passing the thread id by reference */
		pthread_create(&tid[i],&attr,runner,(void *) &localtid[i-1]); 
		printf("Main: Created child %d with thread id = %lu\n", i, tid[i]);
        }

	/**
	 * Now join on each thread
	 */
	printf("Main: Parent thread joining on all children\n");
	for (i = 0; i < NUM_THREADS; i++){ 
		pthread_join(tid[i], NULL);
	}

	printf("Main: Parent thread exiting\n");
	exit(0);
}

/**
 * The thread will begin control in this function.
 */
void *runner(void *tid) 
{
	int i;
	i = *((int *) tid);
	printf("Thread ID %d doing some work\n", i);

	/* do some work ... */

	pthread_exit(0);
}

