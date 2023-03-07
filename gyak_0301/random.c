#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 


int main()
{
    int lower = 500, upper = 1000;

    srand(time(0));

    int num = (rand() %
    (upper - lower + 1)) + lower;
    printf("Random number: %d ", num);
 
    return 0;
}