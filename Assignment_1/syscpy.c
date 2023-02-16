#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

int main(int argc, char* argv[]){

    char *inputFile = argv[1];
    char *outputFile = argv[2];

    // Open the input file
    int inputOpened = syscall(SYS_open, inputFile, O_RDONLY);
    if(inputOpened == -1){
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    // Open the output file
    int outputOpened = syscall(SYS_open, outputFile, O_WRONLY | O_CREAT);
    if(outputOpened == -1){
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    char character[1];
    
    while(syscall(SYS_read, inputOpened, character, 1) != 0){
        syscall(SYS_write, outputOpened, character, 1);
    }
    
    // Close input file
    int closeInput = syscall(SYS_close, inputOpened);
    if(closeInput == -1){
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    // Close output file
    int closeOutput = syscall(SYS_close, outputOpened);
    if(closeOutput == -1){
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    
    return 0;
}
