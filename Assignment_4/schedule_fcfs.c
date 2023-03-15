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
    struct node* temp = head;
    while(temp != NULL)
    {
        run(temp->task,temp->task->burst);
        temp = temp->next;
        //delete(&temp, temp->task);
    }
}




