#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

const int N_THREADS = 60;

void calc_for_one_sec(void *_)
{
    pid_t tid;
    time_t start_time, current_time;
    double elapsed_time = 0.0;
    int sum = 0.0, i = 1;

    time(&start_time);

    while (elapsed_time < 1.0)
    {
        sum += i;
        i++;
        sum = floor(sqrt(sum));

        time(&current_time);
        elapsed_time = difftime(current_time, start_time);
    }

    printf("Calculating floor(sqrt(sum + i)) for %d times: %d\n", i, sum);
}

int main(int argc, char *argv[])
{
    double total_time;
    clock_t start, end;
    start = clock();
    pthread_t threads[N_THREADS];
    int i;

    printf(":: Start threads ...\n");
    for (i = 0; i < N_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, calc_for_one_sec, NULL);
    }

    printf(":: Join threads ...\n");
    for (i = 0; i < N_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    printf(":: Ready.\n");
    end = clock();

    total_time = ((double)(end - start)) / CLK_TCK;
    printf("Total time: %f\n", total_time);

    return 0;
}
