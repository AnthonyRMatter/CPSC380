#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "list.h"
#include "cpu.h"

struct node* head = NULL;
int numberOfTasks = 0;

void add(char *name, int priority, int burst)
{
    Task* newTask = malloc(sizeof(Task));

    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    insertBack(&head, newTask);
    numberOfTasks++;

}

void schedule()
{
    struct node* sort = head;
    
    Task* arr[numberOfTasks];
    int i = 0;
    int z = 0; 
    int indicator = 0;
    int length = sizeof(arr)/sizeof(arr[0]);

    /*Store original singly linked list into array*/
    while(sort != NULL)
    {
        arr[i] = malloc(sizeof(Task));
        arr[i]->burst = sort->task->burst;
        arr[i]->priority = sort->task->priority;
        arr[i]->name = sort->task->name;
        i++;
        sort = sort->next;
    }

    /*Utilize insertion sort to sort array based off of CPU bursts*/
    for(int j = 1; j < length; ++j)
    {
        indicator = arr[j]->burst;
        z = j - 1;

        while(z >= 0 && arr[z]->burst > indicator)
        {
            arr[z + 1]->burst = arr[z]->burst;
            arr[z + 1]->name = arr[z]->name;
            arr[z + 1]->priority = arr[z]->priority;
            z -= 1;
        }
        arr[z + 1]->burst = indicator;
    }

    sort = head;
    i = 0;
    /*Adjusts singly linked list to be sorted by CPU burst*/
    while(sort != NULL)
    {
        sort->task->burst = arr[i]->burst;
        sort->task->priority = arr[i]->priority;
        sort->task->name = arr[i]->name;
        i++;
        sort = sort->next;
    }

    struct node* temp = head;
    while(temp != NULL){
        run(temp->task,temp->task->burst);
        temp = temp->next;
    }
    
}