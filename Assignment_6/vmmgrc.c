#include <stdint.h>
#include <stdio.h>
//may need to flip order for little/big endian 
typedef struct{
    __uint32_t unused: 16; 
    __uint32_t pageno: 8; 
    __uint32_t offset: 8; 
} virtualAddress; 
​
typedef union{
    __uint32_t ul; 
    __uint32_t bf; 
}virtualmemory;
​
virtualmemory vaddr; 
​
//fread(&addr, sizeof(int),1,fd); 
vaddr.ul = addr; 
​
​
//fseek(512); 
​
vaddr.bf.pageno;
vaddr.bf.offset;