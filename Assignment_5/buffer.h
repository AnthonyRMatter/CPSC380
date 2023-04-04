#include <stdint.h> 
#ifndef BUFFER.H
#define BUFFER_SIZE 30
#define NUM_ITEMS 15
typedef struct{
    uint16_t cksum;
    uint8_t buffer[BUFFER_SIZE];
} buffer_item; 
#endif
struct buffer_item item[NUM_ITEMS]; 