#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

#define ALPHA 1.0
#define BETA 2.0
#define RHO 0.5
#define Q 100.0

const int N_THREADS = 10;

typedef struct Task
{
    int ant_id;
    int num_ants;
    int num_cities;
    double *city_distances;
    double *pheromones;
    int *ant_tours;
    double *ant_lengths;
} Task;

void init_distance_matrix(const char *distances, Task *task);
void init_pheromones(Task *task);
void init_ants(Task *task);
void *construct_ant_tour(void *arg);
void update_pheromones(int num_cities, int num_ants, double (*pheromones)[num_cities], int (*ant_tours)[num_cities], double *ant_lengths);
void find_best_tour(int num_cities, int num_ants, int (*ant_tours)[num_cities], double *ant_lengths, int *best_tour, double *best_length);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t threads[N_THREADS];
    int num_iterations = 1;
    Task task;
    task.num_ants = N_THREADS;
    task.num_cities = 7;
    task.city_distances = (double *)malloc(task.num_cities * task.num_cities * sizeof(double));
    task.pheromones = (double *)malloc(task.num_cities * task.num_cities * sizeof(double));
    task.ant_tours = (int *)malloc(task.num_ants * task.num_cities * sizeof(int));
    task.ant_lengths = (double *)malloc(task.num_ants * sizeof(double));
    int best_tour[task.num_cities];
    double best_length = INFINITY;

    init_distance_matrix("data/sh07.txt", &task);
    init_pheromones(&task);
    init_ants(&task);

    for (int q = 0; q < num_iterations; q++)
    {
        printf(":: Start threads ...\n");
        for (int j = 0; j < N_THREADS; j++)
        {
            task.ant_id = j;
            pthread_create(&threads[j], NULL, construct_ant_tour, (void*)&task);
            printf("Thread %d created\n", j);
        }

        // update_pheromones(num_cities, num_ants, pheromones, ant_tours, ant_lengths);
        // find_best_tour(num_cities, num_ants, ant_tours, ant_lengths, best_tour, &best_length);
    }

    for (int i = 0; i < task.num_ants; i++)
    {
        for (int j = 0; j < task.num_cities; j++)
        {
            int offset = i * task.num_cities + j;
            printf("%d ", task.ant_tours[offset]);
        }
        printf("\n");
    }
    /*
        printf("\nBest tour: ");
        for (int i = 0; i < task.num_cities; i++)
        {
            printf("%d ", best_tour[i]);
        }
        printf("\nBest tour length: %lf\n", best_length);
    */
    free(task.city_distances);
    free(task.pheromones);
    free(task.ant_tours);
    free(task.ant_lengths);

    printf(":: Join threads ...\n");
    for (int i = 0; i < N_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void init_distance_matrix(const char *distances, Task *task)
{
    FILE *fp;
    if ((fp = fopen(distances, "r")) == NULL)
    {
        printf("File opening error");
        exit(-1);
    }

    int i, j;

    for (i = 0; i < task->num_cities; i++)
    {
        for (j = 0; j < task->num_cities; j++)
        {
            int offset = i * task->num_cities + j;
            fscanf(fp, "%lf", &task->city_distances[offset]);
        }
    }

    fclose(fp);
}

void init_pheromones(Task *task)
{
    for (int i = 0; i < task->num_cities; i++)
    {
        for (int j = 0; j < task->num_cities; j++)
        {
            int offset = i * task->num_cities + j;
            task->pheromones[offset] = 100;
        }
    }
}

void init_ants(Task *task)
{
    for (int i = 0; i < task->num_ants; i++)
    {
        task->ant_tours[i * task->num_cities + 0] = rand() % task->num_cities;
        for (int j = 1; j < task->num_cities; j++)
        {
            int offset = i * task->num_cities + j;
            task->ant_tours[offset] = -1;
        }
        task->ant_lengths[i] = 0;
    }
}

void *construct_ant_tour(void *arg)
{
    Task *task = (Task *)arg;

    double total_probabilities, random_num, probability_sum, probability_to_move;
    int current_city, next_city, next_city_found;
    int offset;
    int *visited_cities;
    visited_cities = (int *)malloc(task->num_cities * sizeof(int));
    if (visited_cities == NULL)
    {
        printf("Memory not allocated.\n");
        exit(0);
    }

    for (int j = 0; j < task->num_cities; j++)
    {
        visited_cities[j] = 0;
    }
    offset = task->ant_id * task->num_cities + 0;
    visited_cities[task->ant_tours[offset]] = 1;
    current_city = task->ant_tours[offset];

    for (int k = 1; k < task->num_cities; k++)
    {
        total_probabilities = 0;
        random_num = (double)rand() / RAND_MAX;
        probability_sum = 0;
        for (int l = 0; l < task->num_cities; l++)
        {
            if (visited_cities[l] == 0)
            {
                offset = current_city * task->num_cities + l;
                total_probabilities += pow(task->pheromones[offset], ALPHA) * pow(1.0 / task->city_distances[offset], BETA);
            }
        }

        next_city_found = 0;
        for (int m = 0; m < task->num_cities; m++)
        {
            if (visited_cities[m] == 0)
            {
                offset = current_city * task->num_cities + m;
                probability_sum += pow(task->pheromones[offset], ALPHA) * pow(1.0 / task->city_distances[offset], BETA);
                probability_to_move = probability_sum / total_probabilities;
                if (probability_to_move >= random_num)
                {
                    next_city_found = 1;
                    next_city = m;
                    break;
                }
            }
        }

        if (next_city_found == 0)
        {
            for (int n = task->num_cities; n >= 0; n--)
            {
                if (visited_cities[n] == 0)
                {
                    next_city = n;
                    break;
                }
            }
        }

        offset = task->ant_id * task->num_cities + k;
        task->ant_tours[offset] = next_city;

        offset = current_city * task->num_cities + next_city;
        task->ant_lengths[task->ant_id] += task->city_distances[offset];

        visited_cities[next_city] = 1;
        current_city = next_city;
    }

    // Adding the distance between the starting and ending city to the length of the tour
    offset = task->ant_id * task->num_cities + (task->num_cities - 1);
    int last_num = task->ant_tours[offset];

    offset = task->ant_id * task->num_cities + 0;
    int first_num = task->ant_tours[offset];

    offset = last_num * task->num_cities + first_num;
    task->ant_lengths[task->ant_id] += task->city_distances[offset];

    free(visited_cities);

    return NULL;
}

void update_pheromones(int num_cities, int num_ants, double (*pheromones)[num_cities], int (*ant_tours)[num_cities], double *ant_lengths)
{
    int city1, city2;
    // Evaporating pheromones
    for (int i = 0; i < num_cities; i++)
    {
        for (int j = 0; j < num_cities; j++)
        {
            pheromones[i][j] *= (1.0 - RHO);
        }
    }
    // Depositing pheromones
    for (int i = 0; i < num_ants; i++)
    {
        for (int j = 0; j < num_cities - 1; j++)
        {
            city1 = ant_tours[i][j];
            city2 = ant_tours[i][j + 1];
            pheromones[city1][city2] += Q / ant_lengths[i];
            pheromones[city2][city1] += Q / ant_lengths[i];
        }
        city1 = ant_tours[i][num_cities - 1];
        city2 = ant_tours[i][0];
        pheromones[city1][city2] += Q / ant_lengths[i];
        pheromones[city2][city1] += Q / ant_lengths[i];
    }
}

void find_best_tour(int num_cities, int num_ants, int (*ant_tours)[num_cities], double *ant_lengths, int *best_tour, double *best_length)
{
    for (int i = 0; i < num_ants; i++)
    {
        if (ant_lengths[i] < *best_length)
        {
            *best_length = ant_lengths[i];
            for (int j = 0; j < num_cities; j++)
            {
                best_tour[j] = ant_tours[i][j];
            }
        }
    }
}