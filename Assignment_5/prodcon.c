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
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


buffer_item buffer[BUFFER_SIZE];
void *Producer(void *param); /*Producer Thread*/
void *Consumer(void *param); /*Consumer Thread*/
int thread;

// unsigned int checksum(unsigned int *ptr, int numberbytes)
// {
//     unsigned long total;
//     for(total = 0; numberbytes > 1; numberbytes -= 2)
//     {
//         total += *ptr++;
//     }
//     if(numberbytes == 1)
//     {
//         total += *(unsigned char*)ptr;
//     }
    
//     total = (total >> 16) + (total & 0xffff);
//     total += (total >> 16);
//     return (unsigned int)(total);
// }

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
    
    //return 0 if successful, -1 if error
}

int remove_item(buffer_item *item){
    //return 0 if successful, -1 if error
}

int main(int argc, char *argv[]){
    //command line arguments 
    int producerThreads = atoi(argv[2]);
    int consumerThreads = atoi(argv[3]);
    //initialize buffer
    buffer_item *item = malloc(sizeof(buffer_item));

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
    //exit 
    return 0; 
}


void *Producer(void *param)
{
    // Do work
}

void *Consumer(void *param)
{
    // Do work
}