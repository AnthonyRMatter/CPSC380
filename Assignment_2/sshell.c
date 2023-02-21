#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command*/
/*Fork a child process and that child process will invoke execvp*/
/*When checking for &, always assume that it will be at the end*/
/*We are calling the system to invoke commands*/
/*When user input is parsed, make sure there is a null character after string
(this will terminate the string)*/
int main(void){
    char *args[MAX_LINE/2 + 1]; /* command line args */
    int should_run = 1; /* flag to determine when to exit*/
    while (should_run){
        printf("osh>");
        fflush(stdout);
        // read user input
        // fork child process using fork()
        // the child process invokes execvp()
        // parent can invoke wait()
            // This is if the parent sees &
    }
    return 0; 
}