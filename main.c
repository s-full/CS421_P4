#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

typedef int buffer_item;
#define BUFFER_SIZE 5
#define EMPTY -1

buffer_item buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int in = 0;
int out = 0;

int running = 1;

void print_buffer() {

    // prints buffer, prints empty if value is -1
    
    printf("\t[");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == EMPTY) {
            printf("empty");
        } else {
            printf("%d", buffer[i]);
        }
        if (i < BUFFER_SIZE - 1) {
            printf("][");
        }
    }
    printf("] in = %d, out = %d\n", in, out);

}

int insert_item(int item) 
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    // critical section

    
    buffer[in] = item;
   

    printf("insert_item inserted item %d at position %d", item, in);
    in = (in + 1) % BUFFER_SIZE;
    print_buffer();

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

    if (buffer[out] == EMPTY) {
        return -1;
    }

    buffer_item item = buffer[out];
    buffer[out] = EMPTY;
    out = (out+1) % BUFFER_SIZE;

    printf("remove_item removed item %d at position %d", item, out);
    print_buffer();

    // end critical section

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return item;

}


void *producer(void *arg)
{
    pthread_t tid = pthread_self();
    printf("Creating producer thread with id %lu\n", tid);
    buffer_item item;
    while (running)
    {
        srand(time(NULL));
        int sleepy_time = rand() % 3 +1;
        printf("Producer thread %lu sleeping for %d seconds\n ", tid, sleepy_time);
        sleep(sleepy_time);

        item = rand()%100+1;
        printf("Producer thread %lu inserted %d\n", tid, item);
        insert_item(item);   
    }

    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    pthread_t tid = pthread_self();
    printf("Creating consumer thread with id %lu\n", tid);
    buffer_item item;
    while (running)
     {
          srand(time(NULL));
          int sleepy_time = rand() % 3 +1;
          printf("Consumer thread %lu sleeping for %d seconds\n", tid, sleepy_time);
          sleep(sleepy_time);

          item = remove_item();  
    }

    pthread_exit(NULL);
    
}

int main(int argc, char *argv[]) 
{
    in = 0;
    out = 0;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = EMPTY;
    }

/* 1. Get command line arguments */
/* 2. Initialize buffer */
/* 3. Create producer threads. */
/* 4. Create consumer threads. */
/* 5. Sleep. */
/* 6. Main thread exits. */

    // if (argc != 4) {
    //     printf("Invalid args\n");
    //     exit(-1);
    // }

    //get cmd line args
    int time = atoi(argv[1]);
    int num_prods = atoi(argv[2]);
    int num_consums = atoi(argv[3]);

    // alloc threads
    pthread_t *consumer_threads;// = malloc(sizeof(pthread_t) * num_consums);
    pthread_t *producer_threads;// = malloc(sizeof(pthread_t) * num_prods);

    consumer_threads = (pthread_t *) malloc(num_consums * sizeof(pthread_t));
    producer_threads = (pthread_t *) malloc(num_prods * sizeof(pthread_t));

    // check if threads not allocated
    // if (consumer_threads == NULL || producer_threads == NULL)
    // {
    //     printf("ERROR: failed to create threads");
    //     exit(1);
    // }

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
        unsigned long rc = pthread_create(&consumer_threads[i],NULL, consumer, NULL);
        if (rc) {
            printf("ERROR: Failed in creating consumer thread %d", i);
            exit(3);
        }
        
    }

    printf("Main thread sleep for %d seconds", time);
    sleep(time);
    running = 0;

    printf("Main thread exiting\n");

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    // //join producer threads
    //  for (int i = 0; i < num_prods; i++) {
    //     pthread_join(producer_threads[i], NULL);
       
    // }

    //  // join consumer threads
    // for (int i = 0; i < num_consums; i++) {
    //     pthread_join(consumer_threads[i], NULL);
    // }
    exit(0);
}