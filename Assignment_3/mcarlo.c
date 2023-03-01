#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>

int npoints, thread;
int hit_count = 0;
double x, y;
void *runner(void *param); /* the thread */

//First Thread -> Parent Process
//Second Thread -> Makes the calculations

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}

int main(int argc, char* argv[])
{
    npoints = atoi(argv[1]);

    pthread_t tid; 
    pthread_attr_t attr;

    printf("Number of Points: %d\n", npoints);

    pthread_attr_init(&attr);
    thread = pthread_create(&tid, &attr, runner, NULL);

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

    printf("Number of Points Inside Circle: %d\n", hit_count);

    // Calculate PI with updated value of hit_count
    double pi = 4 * ((double)hit_count/npoints);

    printf("Estimate of Pi: %f\n", pi);

    return 0;
}

void *runner(void *param)
{
    for (int i = 0; i < npoints; ++i)
        {
            /* Generates random numbers between -1.0 and +1.0 (exclusive) */
            x = random_double() * 2.0 - 1.0;
            y = random_double() * 2.0 - 1.0;

            if(sqrt(x*x + y*y) < 1.0)
            {
                ++hit_count;
            }
        }
    pthread_exit(0);
}