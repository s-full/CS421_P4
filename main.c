#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

typedef int buffer_item;
#define BUFFER_SIZE 5
#define EMPTY -1

buffer_item buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int in = 0;
int out = 0;

int insert_item(int item) 
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    // critical section

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;

    // end critical section

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return 0;
}

int remove_item() 
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    // critical section

    buffer_item item = buffer[out];
    buffer[out] = EMPTY;
    out = (out+1) % BUFFER_SIZE;

    // end critical section

    pthread_mutex_lock(&mutex);
    sem_post(&empty);

    return item;

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
    
    buffer_item item;
    while (true)
     {
          srand(time(NULL));
          int sleepy_time = rand() % 3 +1;
          sleep(sleepy_time);

         item = remove_item();  
    }
    
}

main(int argc, char *argv[]) 
{
    in = 0;
    out = 0;
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
    int time = argv[1];
    int num_prods = argv[2];
    int num_consums = argv[3];

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

    sleep(time);

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