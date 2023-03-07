#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 

int main() {

 srand(time(0)); 
 int upper = 1000, lower = 500;
 float range = upper - lower;
 float div = RAND_MAX / range;
 float value;

 value = lower + (rand()/div);

 printf("Random float: %f", value);
 }