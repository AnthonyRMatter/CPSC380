#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>

/* THIS CODE HAS THE LOGIC, BUT IT NEEDS TO HAVE THREADS INCLUDED*/

void *runner(void *param); /* the thread */

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}

int main(int argc, char* argv[])
{
    int hit_count = 0;
    int npoints = atoi(argv[1]);
    printf("%d\n", npoints);
    /*Check for points inside circle*/
    int i;
    double x, y;
    for (i = 0; i < npoints; ++i)
    {
        /* Generates random numbers between -1.0 and +1.0 (exclusive) */
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;

        if(sqrt(x*x + y*y) < 1.0)
        {
            ++hit_count;
        }
    }

    printf("%d\n", hit_count);
    double pi = 4 * ((double)hit_count/npoints);
    printf("%f\n", pi);
    return 0;
}