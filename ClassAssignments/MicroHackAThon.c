#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>


void *average(void *param); /* the thread */
void *minimum(void *param);
void *maximum(void *param);
int aver;
int min, max;
int thread, thread1, thread2;
int length;
int arrayThing [10];




//First Thread -> Parent Process
//Second Thread -> Makes the calculations

/* Generates a double precision random number */

int main(int argc, char* argv[])
{
    length = argc - 1;
    for (int i = 0; i < argc-1; i++){
        arrayThing[i] = atoi(argv[i+1]);
        //printf("%d\n",arrayThing[i]);
    }

    pthread_t tid;
    pthread_t tid1; 
    pthread_t tid2; 

    pthread_attr_t attr;
    pthread_attr_t attr1;
    pthread_attr_t attr2;


    pthread_attr_init(&attr);
    thread = pthread_create(&tid, &attr, average, NULL);

    pthread_attr_init(&attr1);
    thread1 = pthread_create(&tid1, &attr1, minimum, NULL);

    pthread_attr_init(&attr2); 
    thread1 = pthread_create(&tid2, &attr2, maximum, NULL);


    // Check if thread was not created
    if(thread != 0)
    {
        perror("Error: Thread failed to produce");
        exit(EXIT_FAILURE);
    }

    thread = pthread_join(tid, NULL);

    // Check if thread did not join
    if(thread != 0)
    {
        perror("Error: Thread failed to join");
        exit(EXIT_FAILURE);
    }

    printf("The average value is  %d\n", aver);
    printf("The minimum value is  %d\n", min);
    printf("The maximum value is  %d\n", max);



    return 0;
}

void *average(void *param)
{
    for (int i = 0; i < length; ++i)
        {
            aver += arrayThing[i];
        }
    aver /= length;

    pthread_exit(0);
}

void *minimum(void *param)
{
    min = arrayThing[0];

    for (int i = 0; i < length; ++i)
    {
        if(min > arrayThing[i])
        {
            min = arrayThing[i];
        }
    }
    pthread_exit(0);
}

void *maximum(void *param)
{
    max = arrayThing[0];

    for (int i = 0; i < length; ++i)
    {
        if(max < arrayThing[i])
        {
            max = arrayThing[i];
        }
    }
    pthread_exit(0);
}
