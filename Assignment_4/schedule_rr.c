#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "list.h"
#include "cpu.h"

struct node* head = NULL;

void add(char *name, int priority, int burst)
{
    Task* newTask = malloc(sizeof(Task));

    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    insertBack(&head, newTask);
}

void schedule()
{
    struct node* sort = head;
    int burstTime = 0;

    /*Traverse through the list*/
    while(sort != NULL) // LOOK OUT FOR: Might have to change this to a different node, since the length of this node might alter within the loop
    {
        /*If the burst is bigger than time quantum, set burst to remainder and put remainder at end of list*/
        //printf("Check Contents of Burst: %d\n", sort->task->burst);
        burstTime = sort->task->burst;
        if(burstTime - QUANTUM > 0)
        {
            //printf("Test\n");
            /*Create new node with all contents of node that has bigger burst than Time Quantum*/
            //printf("Test\n");
            // newList->task->priority = sort->task->priority;
            // newList->task->name = sort->task->name;
            // newList->task->burst = sort->task->burst - QUANTUM;
            add(sort->task->name, sort->task->priority, sort->task->burst - QUANTUM);
            // printf("Contents of Kicked-Out Node:\n");
            // printf("Name: %s\n", newList->task->name);
            // printf("Burst: %d\n", newList->task->burst);
            // printf("Priority: %d\n", newList->task->priority);
            // tail->next = newList; // Set new end of list
            // printf("Task burst: %d\n", tail->next->task->burst);

            sort->task->burst = QUANTUM;
        }

        sort = sort->next;
    }
    
    struct node* temp = head;
    while(temp != NULL)
    {
        run(temp->task,temp->task->burst);
        temp = temp->next;
    }
    
}