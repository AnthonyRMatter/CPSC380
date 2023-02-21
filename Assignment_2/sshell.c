#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command*/

/*Fork a child process and that child process will invoke execvp*/
/*When checking for &, always assume that it will be at the end*/
/*We are calling the system to invoke commands*/
/*When user input is parsed, make sure there is a null character after string
(this will terminate the string)*/
// Use strtok
int main(void){
    char *args[MAX_LINE/2 + 1]; /* command line args */
    int should_run = 1; /* flag to determine when to exit*/
    while (should_run){
        printf("osh>");
        fflush(stdout);
        // read user input

        // error handling 

        // fork child process using fork()
        pid_t pid;
	    pid = fork();
    
        // the child process invokes execvp()
        if (pid == 0) { /* child process */
            // execvp(args[0], args);
            return 0;
        }
        // parent can invoke wait() if last arg is &
        else if (pid > 0) { /* parent process */
            wait(NULL);
            return 0;
        } 
    }

    return 0; 
}