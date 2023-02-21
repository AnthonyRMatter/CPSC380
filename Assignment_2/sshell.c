#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


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
    int i = 0;
    int should_run = 1; /* flag to determine when to exit*/
    while (should_run){
        printf("osh> ");
        fflush(stdout);
        
        // read user input
        char inputs[MAX_LINE];
        fgets(inputs, sizeof(inputs), stdin);

        //tokenization
        const char delimiter[2] = " ";

        args = strtok(inputs, delimiter);
        while(args != NULL){
            argarray[i++] = args;
            args = strtok(NULL, delimiter);
        }

        for(int i = 0; i < 2; ++i){
            printf("%s\n", argarray[i]);
            if(argarray[i] == NULL){
                break;
            }
        }

        // error handling
        if(argarray[0] == "exit"){
            exit(0);
        }



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
            int j = execvp(argarray[0], argarray);
            
            printf("%d\n", j);
            return 0;
        }
        // parent can invoke wait() if last arg is &
        else if (pid > 0) { /* parent process */
            int index = 0;
            while(argarray[index] != NULL){ // Traverse through user input
                if(argarray[index] == "&"){ // If there is an &, 
                    wait(NULL);
                    return 0;
                }
                index++;
            }
            // wait(NULL);
            // return 0;
        } 
    }

    return 0; 
}