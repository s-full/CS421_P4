#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

#include <buffer.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int insert_item(int item) 
{

    return 0;
}

void remove_item() 
{

}


void *producer(void *arg)
{
    buffer_item item;
    while (true)
    {
        srand(time(NULL));
        int sleepy_time = rand() % 3 +1;
        sleep(sleepy_time);

        item = rand()%100+1;
        printf("producer produced %d\n", item);
        if (insert_item(item) < 0) {
            printf("Producer error\n");
        }   
    }
}

void *consumer(void *arg)
{

}

main(int argc, char *argv[]) 
{

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

/* 1. Get command line arguments */
/* 2. Initialize buffer */
/* 3. Create producer threads. */
/* 4. Create consumer threads. */
/* 5. Sleep. */
/* 6. Main thread exits. */

    if (argc != 3) {
        printf("Invalid args\n");
        exit(-1);
    }

    // get cmd line args
    int time = argv[0];
    int num_prods = argv[1];
    int num_consums = argv[2];

    // alloc threads
    pthread_t *consumer_threads = malloc(sizeof(pthread_t) * num_consums);
    pthread_t *producer_threads = malloc(sizeof(pthread_t) * num_prods);

    // check if threads not allocated
    if (consumer_threads == NULL || producer_threads == NULL)
    {
        printf("ERROR: failed to create threads");
        exit(1);
    }

    // create prod threads
   
    for (int i = 0; i < num_prods; i++) {
        int rc = pthread_create(&producer_threads[i], NULL, producer, NULL);
        if (rc) {
            printf("ERROR: Failed in creating producer thread %d", i);
            exit(2);
        }
    }
    // create consum threads
    for (int i = 0; i < num_prods; i++) {
        int rc = pthread_create(&consumer_threads[i],NULL, consumer, NULL);
        if (rc) {
            printf("ERROR: Failed in creating consumer thread %d", i);
            exit(3);
        }
    }

    // join producer threads
    for (int i = 0; i < num_prods; i++) {
        int rc = pthread_join(&producer_threads[i], NULL);
        if (rc) {
            printf("ERROR: Failed in joining producer thread %d", i);
            exit(4);
        }
    }

    // join consumer threads
    for (int i = 0; i < num_consums; i++) {
        int rc = pthread_join(&consumer_threads[i], NULL);
        if (rc) {
            printf("ERROR: Failed in creating producer thread %d", i);
            exit(5);
        }
    }
    return 0;
}