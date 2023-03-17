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
    while(sort != NULL)
    {
        /*If the burst is bigger than time quantum, set burst to remainder and put remainder at end of list*/
        burstTime = sort->task->burst;

        if(burstTime - QUANTUM > 0)
        {
            add(sort->task->name, sort->task->priority, sort->task->burst - QUANTUM);

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