#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


#define MAX_LINE 80 /* The maximum length command*/

/*Fork a child process and that child process will invoke execvp*/
/*When checking for &, always assume that it will be at the end*/
/*We are calling the system to invoke commands*/
/*When user input is parsed, make sure there is a null character after string
(this will terminate the string)*/
// Use strtok
int main(void){
    char *argarray[MAX_LINE/2 + 1]; 
    char *args; /* command line args */
    int i;
    int hasAmpercand;
    int should_run = 1; /* flag to determine when to exit*/
    while (should_run){
        printf("osh> ");
        fflush(stdout);
        
        // read user input
        char inputs[MAX_LINE];
        fgets(inputs, sizeof(inputs), stdin);

        int length = strlen(inputs);
        inputs[length - 1] = '\0'; // Ensures that the end of the input is null terminated

        // strip newline char
        inputs[strcspn(inputs, "\n")] = 0; 

        //tokenization
        const char delimiter[2] = " ";
        i = 0;
        hasAmpercand = 0; 

        args = strtok(inputs, delimiter);
        while(args != NULL){
            if(strcmp(args,"&") == 0){
                hasAmpercand = 1;
                args = strtok(NULL, delimiter);
            }
            else
            {
                argarray[i++] = args;
                args = strtok(NULL, delimiter);
            }

        }

        //null-terminated array for execvp
        argarray[i] = NULL; // We can now traverse through this array because we know that NULL marks the end of array

        // fork child process using fork()
        pid_t pid;
	    pid = fork();
    
        // the child process invokes execvp()
        if(pid < 0){
            /*Error occurred when forking*/
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid == 0) { /* child process */

            execvp(argarray[0], argarray);

            return 0;
        }
        // parent can invoke wait() if last arg is &
        else if (pid > 0) { /* parent process */
            int index = 0;
            while(argarray[index] != NULL){ // Traverse through user input
                if(hasAmpercand){
                    wait(NULL);
                }

                else if (strcmp(argarray[index], "exit") == 0){
                    should_run = 0;
                    break;
                }
                index++;
            }
        } 
    }

    return 0; 
}