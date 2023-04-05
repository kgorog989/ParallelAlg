#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_array(int *array, int lower, int upper, int size);
int array_sum(int *array, int size);
int array_min(int *array, int size);
int array_max(int *array, int size);
int recursive_array_sum(int *array, int size);
int recursive_array_min(int *array, int size);
int recursive_array_max(int *array, int size);
int max(int a, int b);
int min(int a, int b);

int main()
{
    int lower = 5, upper = 1000000, size = 50000;
    int array[size];
    int sum, max, min, rec_sum, rec_max, rec_min;
    FILE *min_max_times;
    double total_time;
    clock_t start, end;
    min_max_times = fopen("min_max_times.txt", "w");

    fill_array(array, lower, upper, size);

    start = clock();
    sum = array_sum(array, size);
    end = clock();
    total_time = ((double)(end - start)) / CLK_TCK;
    fprintf(min_max_times, "array_sum %f\n", total_time);

    start = clock();
    rec_sum = recursive_array_sum(array, size);
    end = clock();
    total_time = ((double)(end - start)) / CLK_TCK;
    fprintf(min_max_times, "recursive_array_sum %f\n", total_time);

    start = clock();
    max = array_max(array, size);
    end = clock();
    total_time = ((double)(end - start)) / CLK_TCK;
    fprintf(min_max_times, "array_max %f\n", total_time);

    start = clock();
    rec_max = recursive_array_max(array, size);
    end = clock();
    total_time = ((double)(end - start)) / CLK_TCK;
    fprintf(min_max_times, "recursive_array_max %f\n", total_time);

    start = clock();
    min = array_min(array, size);
    rec_sum = recursive_array_sum(array, size);
    end = clock();
    total_time = ((double)(end - start)) / CLK_TCK;
    fprintf(min_max_times, "array_min %f\n", total_time);

    start = clock();
    rec_min = recursive_array_min(array, size);
    end = clock();
    total_time = ((double)(end - start)) / CLK_TCK;
    fprintf(min_max_times, "recursive_array_min %f\n", total_time);

    printf("Array sum: %d, \nArray sum with recursion: %d, \nArray min: %d, \nArray min with recursion: %d, \nArray max: %d\nArray max with recursion: %d \n", sum, rec_sum, min, rec_min, max, rec_max);
    fclose(min_max_times);

    return 0;
}

void fill_array(int *array, int lower, int upper, int size)
{
    if (size < 1)
    {
        printf("Error. Array must have more elements than 0.");
        exit(1);
    }

    if (lower > upper)
    {
        printf("Error. Lower interval is higher than upper interval.");
        exit(1);
    }

    int i, num;
    srand(time(0));

    for (i = 0; i < size; i++)
    {
        num = (rand() %
               (upper - lower + 1)) +
              lower;
        array[i] = num;
    }
}

int array_sum(int *array, int size)
{
    int i, sum = 0;

    for (i = 0; i < size; i++)
    {
        sum += array[i];
    }

    return sum;
}

int array_min(int *array, int size)
{
    int i, min = array[0];

    for (i = 1; i < size; i++)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    return min;
}

int array_max(int *array, int size)
{
    int i, max = array[0];

    for (i = 1; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    return max;
}

int recursive_array_sum(int *array, int size)
{
    if (size <= 0)
        return 0;
    return (recursive_array_sum(array, size - 1) + array[size - 1]);
}

int recursive_array_min(int *array, int size)
{
    if (size == 1)
        return array[0];
    return min(array[size - 1], recursive_array_min(array, size - 1));
}

int recursive_array_max(int *array, int size)
{
    if (size == 1)
        return array[0];
    return max(array[size - 1], recursive_array_max(array, size - 1));
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}