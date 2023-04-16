#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

const int N_THREADS = 50;

int calc_odd(int *array, int size);
void fill_array(int *array, int size, int lower, int upper);
int calc_odd_p(int *array, int size);

int main(int argc, char *argv[])
{
    int size = 50000, lower = 10, upper = 1000000;
    int array[size];
    double total_time_seq;
    clock_t start_seq, end_seq;
    int i;
    int odd_seq, odd_p;

    fill_array(array, size, lower, upper);

    start_seq = clock();
    odd_seq = calc_odd(array, size);
    printf("Odd numbers (seq): %d\n", odd_seq);
    end_seq = clock();
    total_time_seq = ((double)(end_seq - start_seq)) / CLK_TCK;
    printf("Total time (seq): %f\n", total_time_seq);

    odd_p = calc_odd_p(array, size);
    printf("Odd numbers(parallel): %d\n", odd_p);

    return 0;
}

int calc_odd(int *array, int size)
{
    int i, odd_num = 0;
    for (i = 0; i < size; i++)
    {
        if (array[i] % 2 != 0)
        {
            odd_num++;
        }
    }

    return odd_num;
}

int calc_odd_p(int *array, int size)
{
    int odd = 0;
    int i, j;
    double total_time_p;
    clock_t start_p, end_p;
    pthread_t threads[N_THREADS];

    int small_array_size = size / N_THREADS;
    int num_arrays = N_THREADS;

    int **small_arrays = (int **)malloc(num_arrays * sizeof(int *));

    for (i = 0; i < num_arrays; i++)
    {
        small_arrays[i] = (int *)malloc(small_array_size * sizeof(int));

        for (j = 0; j < small_array_size; j++)
        {
            small_arrays[i][j] = array[i * small_array_size + j];
        }
    }

    start_p = clock();
    printf(":: Start threads ...\n");
    for (i = 0; i < N_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, odd += calc_odd(small_arrays[i], small_array_size), NULL);
    }

    printf("Number of all odds (parallel): %d\n", odd);
    printf(":: Ready.\n");
    end_p = clock();

    for (i = 0; i < num_arrays; i++)
    {
        free(small_arrays[i]);
    }
    free(small_arrays);

    total_time_p = ((double)(end_p - start_p)) / CLK_TCK;
    printf("Total time (parallel): %f\n", total_time_p);

    printf(":: Join threads ...\n");
    for (i = 0; i < N_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return odd;
}

void fill_array(int *array, int size, int lower, int upper)
{
    int i;

    srand(time(0));

    for (i = 0; i < size; i++)
    {
        array[i] = (rand() % (upper - lower + 1)) + lower;
    }
}
