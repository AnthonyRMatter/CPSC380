// use posix semaphores -> named for mac 
//delete named semaphore at the start
//empty semaphore 
//full semaphore
//mutex

// at least 3 threads 
// main, producer (can have more), consumer (can have more)
//producer: random data -> data = rand () % 255; 
//consumer: 

//buffer 
//set number of num_items yourself 
//checksum 

#include "buffer.h"
#include <stdio.h>
#include <fcntl.h>          
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>


buffer_item items[NUM_ITEMS];
void *Producer(void *param); /*Producer Thread*/
void *Consumer(void *param); /*Consumer Thread*/
int thread;
sem_t *full;
sem_t *empty;
pthread_mutex_t binary_mutex;
int in = -1; 
int out = -1;
//int run = 1; 

uint16_t checksum(char *addr, uint32_t count)
{
    register uint32_t sum = 0;
    uint16_t *buf = (uint16_t *)addr;
    while(count > 1){
        sum += *(buf)++;
        count -= 2;
    }

    // Add left-over byte, if any
    if(count > 0){
        sum += *addr;
    }

    // Fold 32-bit sum to 16 bits
    while(sum >> 16){
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (~sum);
}

int insert_item(buffer_item *item){
    //check if full
    if((in == out +1) || (in == 0 && out == NUM_ITEMS)){
        printf ("buffer full\n"); 
        return -1; 
    }
    if(in == -1){
        in = 0; 
    }
    out = (out +1) % NUM_ITEMS; 
    items[out] = *item;
    printf("%d\n",(int)items[out].buffer);
    printf("inserted\n");
    return 0;
}

int remove_item(buffer_item *item){
    printf("%d\n",(int)items[in].buffer);
    //check if empty
    if(in == -1){
        printf ("buffer empty\n"); 
        return -1; 
    } 
    if(in ==out){
        in = -1;
        out = -1;
    }
    else{
        in = (in +1) % NUM_ITEMS; 
    }
    printf("removed\n");
    return 0;
}

int main(int argc, char *argv[]){
     
    //delete any named semaphores just in case
    sem_unlink("Full");
    sem_unlink("Empty");

    //command line arguments 
    if(argc < 4){
        printf("Missing arguments\n");
        exit(1);
    }
    int producerThreads = atoi(argv[2]);
    int consumerThreads = atoi(argv[3]);

    //thread array
    pthread_t *thread_array;
    int nthreads = producerThreads + consumerThreads;
    long *thread_ids;

    thread_array = malloc(nthreads * sizeof(pthread_t));
    thread_ids = malloc(nthreads * sizeof(long));


    // Create Semaphores
    full = sem_open("Full", O_CREAT, 0666, NUM_ITEMS);
    printf("Created full semaphore\n");
    if(full == SEM_FAILED)
    {
        fprintf(stderr, "sem_open() failed. errno: %d\n", errno);
    }

    empty = sem_open("Empty", O_CREAT, 0666, 0);
    printf("Created empty semaphore\n");
    if(empty == SEM_FAILED)
    {
        fprintf(stderr, "sem_open() failed. errno: %d\n", errno);
    }

    //initialize buffer 
    buffer_item *item = malloc(sizeof(buffer_item)); // maybe

   //Create Producer and Consumer Thread IDs and Attributes
    pthread_t producerID[producerThreads];
    pthread_t consumerID[consumerThreads];
    pthread_attr_t producerAttr;
    pthread_attr_t consumerAttr;

    // Create Producer Threads
    for(int i = 0; i < producerThreads; ++i)
    {
        pthread_attr_init(&producerAttr);
        thread_ids[i]= i;
        thread = pthread_create(&thread_array[i], &producerAttr, Producer, NULL); //use an array to track number of threads 
    }
    // Create Consumer Threads
    for(int i = producerThreads; i < nthreads; ++i)
    {
        pthread_attr_init(&consumerAttr);
        thread_ids[i]= i;
        thread = pthread_create(&thread_array[i], &consumerAttr, Consumer, NULL); //use an array to track number of threads
    }

    //sleep 
    sleep(atoi(argv[1]));

    /* Cancel the threads */
    for (int i = 0; i < nthreads; i++) {
        pthread_cancel(thread_array[i]);
    }

    /* Joined the threads */
    for (int i = 0; i < nthreads; i++) {
        pthread_join(thread_array[i], NULL);
    }
    
    // Delete Named Semaphores
    sem_close(full);
    sem_unlink("Full");
    sem_close(empty);
    sem_unlink("Empty");

    //exit
    return 0; 
}


void *Producer(void *param){
    //allocate buffer item
    buffer_item item; 
    while(1){
        int producedData = 0; // Not sure if this goes in here
        for(int i = 0; i < BUFFER_SIZE; ++i){
            producedData = rand() % 256;
            item.buffer[i] = (uint8_t) producedData;
        }

        printf("produced\n"); 

        //enter critical section
        sem_wait(empty);
        pthread_mutex_lock(&binary_mutex);
        // calculate checksum 
        item.cksum = checksum((char *)item.buffer, BUFFER_SIZE);

        // add produced to buffer
        insert_item(&item);
    
        pthread_mutex_unlock(&binary_mutex);
        sem_post(full);
        //exit critical section
    }
    pthread_exit(NULL);
}

void *Consumer(void *param){
    buffer_item item; 
    while(1){

        //enter critical section
        sem_wait(full);
        pthread_mutex_lock(&binary_mutex);

        // Extract item from buffer to consumer buffer
        item = items[in];

        if(item.cksum != checksum((char *)item.buffer, BUFFER_SIZE)){
            printf("data corrupted\n");
            printf("expected %02x, recieved %02x \n", item.cksum, checksum((char *)item.buffer, BUFFER_SIZE));
            EXIT_FAILURE;
        }        

        remove_item(&item);
        pthread_mutex_unlock(&binary_mutex);
        sem_post(empty);
        printf("consumed\n");
        // Consume item next up in consumer buffer

        //exit critical section
    }
    pthread_exit(NULL);
}