#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float generate_float(int lower, int upper);

int main()
{

    int upper = 1000, lower = 500;
    float value = generate_float(lower, upper);

    printf("Random float: %f", value);
}

float generate_float(int lower, int upper)
{
    srand(time(0));

    float range = upper - lower;
    float div = RAND_MAX / range;
    float value = lower + (rand() / div);

    return value;
}