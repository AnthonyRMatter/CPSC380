#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "list.h"
#include "cpu.h"

struct node** head;

void add(char *name, int priority, int burst)
{
    Task* newTask = malloc(sizeof(Task));

    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    insertBack(head, newTask);

}

void schedule()
{
    struct node* tail = *head;

    /*This will set a tail pointer to the end of the list*/
    while(tail->next != NULL)
    {
        tail = tail->next;
    }

    struct node* sort = *head;

    /*Traverse through the list*/
    while(sort != NULL) // LOOK OUT FOR: Might have to change this to a different node, since the length of this node might alter within the loop
    {
        /*If the burst is bigger than time quantum, set burst to remainder and put remainder at end of list*/
        if(sort->task->burst - QUANTUM > 0)
        {
            /*Create new node with all contents of node that has bigger burst than Time Quantum*/
            struct node* remainder = tail->next;
            remainder->task->priority = sort->task->priority;
            remainder->task->name = sort->task->name;
            remainder->task->burst = sort->task->burst - QUANTUM;
            printf("Contents of Remainder Node:\n");
            printf("Name: %s", remainder->task->name);
            print("Burst: %d", remainder->task->burst);
            print("Priority: %d", remainder->task->priority);
            tail = remainder; // Set new end of list
            printf("Task burst: %d\n", sort->task->burst);
            sort->task->burst = QUANTUM;
        }
        sort = sort->next;
    }
    
    struct node* temp = *head;
    while(temp != NULL)
    {
        run(temp->task,temp->task->burst);
        temp = temp->next;
    }
    
}