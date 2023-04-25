#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_ENTRIES 256
#define PAGE_SIZE 256
#define TLB_ENTRIES 16
#define FRAME_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536
#define NUMBER_OF_FRAMES 256


// File declarations
FILE* addresses;
FILE* Backing_Store;

// Paging Statistic Declarations
int page_fault_rate = 0;
int TLB_Hit_Rate = 0;
int page_faults = 0;
int page_hits = 0;
int TLB_Hits = 0;
int TLB_Faults = 0;

// Paging Checkers Declarations
int TLBChecker = 0;
int PageTableChecker = 0;



//may need to flip order for little/big endian 
typedef struct{
    __uint32_t unused: 16; 
    __uint32_t pageno: 8; 
    __uint32_t offset: 8; 
} virtualAddress; 

typedef union{
    __uint32_t ul; 
    virtualAddress bf; 
}virtualmemory;

virtualmemory vaddr; 

struct TLB{
    int page_number;
    int frame_number;
};

struct PageTable{
    int page_number;
    int frame_number;
};

struct TLB tlb[TLB_ENTRIES];

struct PageTable Page_Table[PAGE_ENTRIES];

int Physical_Memory[NUMBER_OF_FRAMES * FRAME_SIZE];

void initializeTLBTable(){
    for(int i = 0; i < PAGE_ENTRIES; ++i)
    {
        Page_Table[i].page_number = -1;
    }
}

void initializePageTable(){
    for(int i = 0; i < TLB_ENTRIES; ++i)
    {
        tlb[i].page_number = -1;
    }
}

int checkPageTable(int Page_Number, int offset)
{
    for(int i = 0; i < PAGE_ENTRIES; ++i)
    {
        if(Page_Table[i].page_number == Page_Number)
        {
            page_hits++;
            return 0;
        }
    }
    page_faults++;
    return -1;
}

int checkTLB(int Page_Number, int offset)
{
    for(int i = 0; i < TLB_ENTRIES; ++i)
    {
        if(tlb[i].page_number == Page_Number)
        {
            TLB_Hits++;
            return 0;
        }
    }
    TLB_Faults++;
    return -1;
}

int main(int argc, char* argv[])
{
    // Initilize the TLB to -1
    initializeTLBTable();

    // Initialize the Page Table to -1
    initializePageTable();

    uint32_t bits;

    // Open addresses.txt
    addresses = fopen(argv[1], "r");

    // Error handle opening the file
    if(addresses == NULL)
    {
        fprintf(stderr, "Error opening %s. Anticipated (name).txt\n", argv[1]);
        return -1;
    }


    char BUFFER[32] = {0};
    // Loop through file
    while(fgets(BUFFER, 32, addresses))
    {
        bits = atoi(BUFFER);
        vaddr.ul = atoi(BUFFER);

        //mask and extract appropriate bits
        vaddr.bf.pageno = ((vaddr.ul & 0xFF0) >> 8);
        vaddr.bf.offset = (vaddr.ul & 0xFF);

        // Evaluate address
        // Check if page number is in TLB
        TLBChecker = checkTLB(vaddr.bf.pageno, vaddr.bf.offset);
        if(TLBChecker == -1)
        {
            // If not, check if page number is in page table
            PageTableChecker = checkPageTable(vaddr.bf.pageno, vaddr.bf.offset);
            if(PageTableChecker == -1)
            {
                // If not, use offset to find and get page number from backing store
                // Swap page in backing store with page in page table
                    // Ideally use fseek() or fopen()/fclose()
                // Translated virtual address from backing store and printed physical address
                printf("Physical Address: %d", vaddr.ul);
            }
            else
            {
                // Translated and printed physical address
                printf("Physical Address: %d", vaddr.ul);
            }
        }
        else
        {
            // Translated and printed physical address
            printf("Physical Address: %d", vaddr.ul);
        }
    }


    // Calculate Page-Fault rate

    // Calculate TLB-Hit Rate


    // close backing store
    fclose(Backing_Store);
    // close addresses.txt
    fclose(addresses);

}