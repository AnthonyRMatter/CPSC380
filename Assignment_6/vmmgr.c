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
#define NUMBER_OF_FRAMES 256
#define BUFFER_SIZE 256
#define PHYSICAL_MEMORY_SIZE NUMBER_OF_FRAMES * FRAME_SIZE



// File declarations
FILE* addresses;
FILE* Backing_Store;

// Paging Statistic Declarations
double page_fault_rate = 0;
double TLB_Hit_Rate = 0;
double page_faults = 0;
int page_hits = 0;
double TLB_Hits = 0;
int TLB_Faults = 0;

// Paging Checkers Declarations
int TLBChecker = 0;
int PageTableChecker = 0;

int8_t Physical_Page[PAGE_SIZE];
uint64_t physical_Address = 0;
int physical_Address_Location = 0;
int8_t byte = 0;

// TLB LRU Variables
int tempPageNumber = 0;
int tempFrameNumber = 0;
int tempValidBit = 0;



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
    int valid_bit;
    int frame_number;
    int page_number;
};
// Since TLB only has 16 entries, page_number needs to be a struct variable

struct PageTable{
    int valid_bit;
    int frame_number;
};
// Index of Page Table represents Page Number

struct TLB tlb[TLB_ENTRIES];

struct PageTable Page_Table[PAGE_ENTRIES];

uint8_t Physical_Memory[PHYSICAL_MEMORY_SIZE]; 
int8_t Physical_Frame_Block = 0;
uint8_t freeFrame = 0;

void initializeTLBTable(){
    for(int i = 0; i < PAGE_ENTRIES; ++i)
    {
        Page_Table[i].valid_bit = -1;
        Page_Table[i].frame_number = -1;
    }
}

void initializePageTable(){
    for(int i = 0; i < TLB_ENTRIES; ++i)
    {
        tlb[i].valid_bit = -1;
        tlb[i].frame_number = -1;
        tlb[i].page_number = -1;
    }
}

int checkPageTable(int Page_Number, int offset)
{
    if(Page_Table[Page_Number].valid_bit == 0)
    {
        page_hits++;
        return 0;
    }
    page_faults++;
    return -1;
}

// If Page Number is in TLB, then return the index of page, if not, return -1
int checkTLB(int Page_Number, int offset)
{
    for(int i = 0; i < TLB_ENTRIES; ++i)
    {
        if(tlb[i].page_number == Page_Number)
        {
            TLB_Hits++;
            return i;
        }
    }
    TLB_Faults++;
    return -1;
}

void moveAndInsertTLBPage(int page_Number, int frame_Number, int valid_Bit)
{
    for(int i = 1; i < TLB_ENTRIES; ++i)
    {
        tlb[i].page_number = tlb[i - 1].page_number;
        tlb[i].frame_number = tlb[i - 1].frame_number;
        tlb[i].valid_bit = tlb[i - 1].valid_bit;
    }

    tlb[0].page_number = page_Number;
    tlb[0].frame_number = frame_Number;
    tlb[0].valid_bit = valid_Bit;
    return;
}

void insertTLBPage(int page_Number, int frame_Number, int valid_Bit)
{
    for(int i = 0; i < TLB_ENTRIES; ++i)
    {
        if(tlb[i].page_number == -1 && tlb[i].frame_number == -1 && tlb[i].valid_bit == -1)
        {
            tlb[i].page_number = page_Number;
            tlb[i].frame_number = frame_Number;
            tlb[i].valid_bit = valid_Bit;
            return;
        }
    }
    return;
}

int checkforOpenTLBPosition()
{
    for(int i = 0; i < TLB_ENTRIES; ++i)
    {
        if(tlb[i].page_number == -1)
        {
            return -1;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    // Initilize the TLB to -1
    initializeTLBTable();

    // Initialize the Page Table to -1
    initializePageTable();

    // Initialize Physical Memory
    for(int i = 0; i < PHYSICAL_MEMORY_SIZE; ++i){
        Physical_Memory[i] = -1;
    }

    uint32_t bits;

    // Open addresses.txt
    addresses = fopen(argv[1], "r");

    if (argc != 2) {
        printf("Error, Command-Lime Parameters Should Be: %s <Name of Text File>\n", argv[0]);
        exit(1);
    }

    // Error handle opening the file
    if(addresses == NULL)
    {
        fprintf(stderr, "Error opening %s. Anticipated addresses.txt\n", argv[1]);
        return -1;
    }

    // Open Backing Store
    Backing_Store = fopen("BACKING_STORE.bin", "rb");

    // error handling
    if(Backing_Store == NULL)
    {
        fprintf(stderr, "Error opening %s.\n", "BACKING_STORE.bin");
        return -1;
    }

    char BUFFER[BUFFER_SIZE] = {0};
    // Loop through file
    while(fgets(BUFFER, BUFFER_SIZE, addresses))
    {
        vaddr.ul = atoi(BUFFER);
        printf("Virtual Address: %d\t", vaddr.ul);

        //extract page number and offset
        vaddr.bf.pageno = ((vaddr.ul & 0xFF00) >> 8);
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
                int findFrame = fseek(Backing_Store, freeFrame * 256, SEEK_SET);

                // Error handling
                if(findFrame != 0)
                {
                    fprintf(stderr, "Couldn't find frame in Backing Store\n");
                }

                fread(Physical_Page, FRAME_SIZE, 1, Backing_Store);

                Physical_Frame_Block = freeFrame * FRAME_SIZE;

                // Allocate memory
                memcpy(&Physical_Memory[Physical_Frame_Block], Physical_Page, FRAME_SIZE);

                freeFrame++;

                Page_Table[vaddr.bf.pageno].frame_number = Physical_Frame_Block;
                Page_Table[vaddr.bf.pageno].valid_bit = 0;

                // Get Location of Byte in Frame given the Offset
                physical_Address_Location = Physical_Frame_Block + vaddr.bf.offset;

                // Get the Physical Address
                physical_Address = (uint64_t)&Physical_Memory[physical_Address_Location];
                byte = (int8_t)Physical_Memory[physical_Address_Location];
            }
            else
            {
                if(checkforOpenTLBPosition() == -1)
                {
                    // TLB has Open Position
                    insertTLBPage(vaddr.bf.pageno, Page_Table[vaddr.bf.pageno].frame_number, Page_Table[vaddr.bf.pageno].valid_bit);
                }
                else
                {
                    moveAndInsertTLBPage(vaddr.bf.pageno, Page_Table[vaddr.bf.pageno].frame_number, Page_Table[vaddr.bf.pageno].valid_bit);
                }

                // Use already stored frame number to find physical address
                physical_Address_Location = (Page_Table[vaddr.bf.pageno].frame_number * FRAME_SIZE) + vaddr.bf.offset;

                // Get the Physical Address
                physical_Address = (uint64_t)&Physical_Memory[physical_Address_Location];                            
                byte = (int8_t)Physical_Memory[physical_Address_Location];
            }
        }
        else
        {   
            // Gets the location of the physical address given the address in the TLB
            physical_Address_Location = (tlb[TLBChecker].frame_number * FRAME_SIZE) + vaddr.bf.offset;                

            // Get the Physical Address
            physical_Address = (uint64_t)&Physical_Memory[physical_Address_Location];
            byte = (int8_t)Physical_Memory[physical_Address_Location];

        }
        // Print Paging Information
        printf("Physical Address: %lu\t", physical_Address);
        printf("Signed Byte: %d\n", byte);
    }

    // Print Page Faults
    printf("\n");
    printf("Statistics:\n");
    // Calculate Page-Fault rate
    page_fault_rate = (page_faults / (page_faults + page_hits))*100;
    printf("Page Fault-Rate: %f%% \n", page_fault_rate);

    // Calculate TLB-Hit Rate
    TLB_Hit_Rate = (TLB_Hits/(TLB_Faults + TLB_Hits))*100;
    printf("TLB Hit-Rate: %f%%\n", TLB_Hit_Rate);

    // close backing store
    fclose(Backing_Store); // seg faults if Backing_Store was not already open
    // close addresses.txt
    fclose(addresses);

}