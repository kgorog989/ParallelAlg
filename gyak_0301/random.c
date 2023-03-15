#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_number(int lower, int upper);

int main()
{
    int lower = 500, upper = 1000;
    int num;

    num = generate_number(lower, upper);

    printf("Random number: %d ", num);

    return 0;
}

int generate_number(int lower, int upper)
{
    srand(time(0));

    int num = (rand() % (upper - lower + 1)) + lower;

    return num;
}