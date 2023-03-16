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
    struct node* temp = head;
    struct node* min = head;
    Task* tempTask = malloc(sizeof(Task));//temp variable to store node data

    while(sort != NULL)
    {
        min = sort;
        temp= sort->next; 
        while (temp !=NULL)//travel till the second last element 
        {
            
           if(min->task->priority > temp->task->priority)
           {
                min = temp;
           }
         temp = temp->next;    // move to the next element 
        }
        tempTask = sort->task;
        sort->task = min->task;
        min->task = tempTask;

        sort = sort->next;    // move to the next node
    }

    struct node* temps = head;
    while(temps != NULL){
        run(temps->task,temps->task->burst);
        temps = temps->next;
    }
    
}