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


buffer_item *items[NUM_ITEMS];
void *Producer(void *param); /*Producer Thread*/
void *Consumer(void *param); /*Consumer Thread*/
int thread;
sem_t *full;
sem_t *empty;
pthread_mutex_t *binary_mutex; 

uint16_t checksum(char *addr, uint32_t count)
{
    register uint32_t sum = 0;

    uint16_t *buf = (uint16_t *)addr;

    while(count > 1)
    {
        sum += *(buf)++;
        count -= 2;
    }

    // Add left-over byte, if any
    if(count > 0)
    {
        sum += *addr;
    }

    // Fold 32-bit sum to 16 bits
    while(sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (~sum);
}

int insert_item(buffer_item *item){
    // Calculate CheckSum
    uint32_t numberbytes = strlen(item->buffer);
    char *ptr = (uint8_t *)item->buffer;
    item->cksum = checksum(ptr, numberbytes);


    // for(int i = 0; i < NUM_ITEMS; ++i)
    // {
    //     if(items[i] == -1) // Check if space in buffer is available
    //     {
    //         items[i] = item; // If so, fill that space with a new item
    //         return 0;
    //     }
    // }
    // return -1;
    
    //return 0 if successful, -1 if error
}

int remove_item(buffer_item *item){
    //return 0 if successful, -1 if error
}

int main(int argc, char *argv[]){
    //command line arguments 
    int producerThreads = atoi(argv[2]);
    int consumerThreads = atoi(argv[3]);

    // Create Semaphores
    full = sem_open("Full", O_CREAT, 0666, NUM_ITEMS);
    empty = sem_open("Empty", O_CREAT, 0666, 0);
    //initialize buffer 
    buffer_item *item = malloc(sizeof(buffer_item)); // maybe

    //Create Producer and Consumer Thread IDs and Attributes
    pthread_t producerID;
    pthread_t consumerID;
    pthread_attr_t producerAttr;
    pthread_attr_t consumerAttr;

    // Create Producer Threads
    while(producerThreads > 0)
    {
        pthread_attr_init(&producerAttr);
        thread = pthread_create(&producerID, &producerAttr, Producer, NULL);
        producerThreads--;
    }

    // Create Consumer Threads
    while(consumerThreads > 0)
    {
        pthread_attr_init(&consumerAttr);
        thread = pthread_create(&consumerID, &consumerAttr, Consumer, NULL);
        consumerThreads--;
    }

    //producer thread calls insert()
    //consumer thread calls remove()
    
    //sleep 
    sleep(atoi(argv[1]));

    // Delete Named Semaphores
    sem_close(full);
    sem_unlink("Full");
    sem_close(empty);
    sem_unlink("Empty");

    //exit
    return 0; 
}


void *Producer(void *param)
{
    while(1)
    {
        // Allocate buffer_item
        buffer_item *item = malloc(sizeof(buffer_item));
        int producedData = 0; // Not sure if this goes in here
        for(int i = 0; i < BUFFER_SIZE; ++i)
        {
            producedData = rand() % 256;
            item->buffer[i] = producedData;

        }
        
        sem_wait(empty);
        pthread_mutex_lock(binary_mutex);

        // add produced to buffer
        insert_item(item);

        pthread_mutex_unlock(binary_mutex);
        sem_post(full);
    }
}

void *Consumer(void *param)
{
    while(1)
    {

        sem_wait(full);
        pthread_mutex_lock(binary_mutex);

        // Extract item from buffer to consumer buffer

        pthread_mutex_unlock(binary_mutex);
        sem_post(empty);

        // Consume item next up in consumer buffer
    }
}