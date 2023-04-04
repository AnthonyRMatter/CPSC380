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
#include<unistd.h>

buffer_item buffer[BUFFER_SIZE]; 

int insert_item(buffer_item *item){
    //return 0 if successful, -1 if error
}

int remove_item(buffer_item *item){
    //return 0 if successful, -1 if error
}
int main(int argc, char *argv[]){
    //command line arguments 
    //initialize buffer 
    //producer thread calls insert()
    //consumer thread calls remove()
    
    //sleep 
    sleep(atoi(argv[1])); 
    //exit 
    return 0; 
}