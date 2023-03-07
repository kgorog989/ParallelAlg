#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_nums(int lower, int upper, int count);

int main() {
int ok, lower, upper, count;
char ch;

do {
 ok = 1;
 printf("How many numbers do you want to generate?: ");
    if (scanf("%d", &count)!=1 || count < 1) {
    printf("Invalid input.\n");
    ok = 0;
    while ((ch=getchar())!='\n') ;
    }
 } while (!ok);

do {
 ok = 1;
 printf("\nLower interval: ");
    if (scanf("%d", &lower)!=1) {
    printf("Invalid input.\n");
    ok = 0;
    while ((ch=getchar())!='\n') ;
    }
 } while (!ok);

 do {
 ok = 1;
 printf("\nUpper interval: ");
    if (scanf("%d", &upper)!=1 || upper<lower) {
    printf("Invalid input\n");
    ok = 0;
    while ((ch=getchar())!='\n') ;
    }
  } while (!ok);

generate_nums(lower, upper, count);

}


void generate_nums(int lower, int upper, int count){
    FILE *generated_numbers;
    generated_numbers = fopen("generated_numbers.txt", "w");

    int i, num;
    srand(time(0));

    for (i = 0; i < count; i++)
    { 
        num = (rand() %
        (upper - lower + 1)) + lower;
        fprintf(generated_numbers, "%d\n", num);
    }
    fclose(generated_numbers);
}