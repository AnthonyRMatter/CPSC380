/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE 100

int main(int argc, char *argv[])
{
    printf("Test\n");
    FILE *in = malloc(sizeof(in));
    char *temp;
    char task[SIZE];
    //char *temp2 = malloc(sizeof(argv[1]));
    //temp2 = argv[1];
    char *inputFile = argv[1];
    printf("Input: %s\n", inputFile);

    char *name;
    int priority;
    int burst;
    
    in = fopen(inputFile,"r");
    // if(in == NULL)
    // {
    //     printf("Error opening file, errno = %d (%s)", errno, strerror(errno));
    //     exit(0);
    // }
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));

        // add the task to the scheduler's list of tasks
        add(name,priority,burst);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}
