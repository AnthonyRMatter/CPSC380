#include <stdio.h>
#include <stdlib.h>

/* THE PORTION BELOW IS JUST A CODE SEGMENT RIGHT NOW, IT WILL CHANGE LATER*/

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}

int main (void)
{
    int x, y, npoints, hit_count;
    /*Check for points inside circle*/
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
}