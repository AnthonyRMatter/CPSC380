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
    //printf("Test\n");

    /*Store original singly linked list into array*/
    while(sort != NULL)
    {
        arr[i] = malloc(sizeof(Task));
        arr[i]->burst = sort->task->burst;
        //printf("Burst: %d\n", arr[i]->burst);
        arr[i]->priority = sort->task->priority;
        //printf("Priority: %d\n", arr[i]->priority);
        arr[i]->name = sort->task->name;
        //printf("Name: %s\n", arr[i]->name);
        i++;
        sort = sort->next;
    }

    int length = numberOfTasks;
    //printf("Length: %d\n", length);

    /*Utilize insertion sort to sort array based off of priority*/
    for(int j = 1; j < length; ++j)
    {
        indicator = arr[j]->priority;
        //printf("Indicator: %d\n", indicator);
        z = j - 1;
        while(z >= 0 && arr[z]->priority > indicator)
        {
            arr[z + 1]->burst = arr[z]->burst;
            arr[z + 1]->name = arr[z]->name;
            arr[z + 1]->priority = arr[z]->priority;
            z -= 1;
        }
        arr[z + 1]->priority = indicator;
    }

    sort = head;
    i = 0;

    /*Adjusts singly linked list to be sorted by priority*/
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