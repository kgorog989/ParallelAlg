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

const int N_THREADS = 50;
int ant_count = 0;

pthread_mutex_t ant_lengths_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ant_tours_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rand_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ant_count_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Task
{
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
void update_pheromones(Task *task);
void find_best_tour(Task *task, int *best_tour, double *best_length);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t threads[N_THREADS];
    int num_iterations = 100;
    Task task;
    task.num_ants = N_THREADS;
    task.num_cities = 312;
    task.city_distances = (double *)malloc(task.num_cities * task.num_cities * sizeof(double));
    task.pheromones = (double *)malloc(task.num_cities * task.num_cities * sizeof(double));
    task.ant_tours = (int *)malloc(task.num_ants * task.num_cities * sizeof(int));
    task.ant_lengths = (double *)malloc(task.num_ants * sizeof(double));
    int best_tour[task.num_cities];
    double best_length = INFINITY;

    init_distance_matrix("data/usca312.txt", &task);
    init_pheromones(&task);
    init_ants(&task);

    for (int q = 0; q < num_iterations; q++)
    {
        ant_count = 0;
        printf(":: Start threads ...\n");
        for (int j = 0; j < N_THREADS; j++)
        {
            pthread_create(&threads[j], NULL, construct_ant_tour, (void *)&task);
            printf("Thread %d created in iteration %d\n", j, q + 1);
        }

        printf(":: Join threads ...\n");
        for (int i = 0; i < N_THREADS; ++i)
        {
            pthread_join(threads[i], NULL);
        }

        update_pheromones(&task);
        find_best_tour(&task, best_tour, &best_length);
    }

    printf("\nBest tour: ");
    for (int i = 0; i < task.num_cities; i++)
    {
        printf("%d ", best_tour[i]);
    }
    printf("\nBest tour length: %lf\n", best_length);

    free(task.city_distances);
    free(task.pheromones);
    free(task.ant_tours);
    free(task.ant_lengths);

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
    int ant_id;

    pthread_mutex_lock(&ant_count_mutex);
    ant_id = ant_count;
    ant_count++;
    pthread_mutex_unlock(&ant_count_mutex);

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
    offset = ant_id * task->num_cities + 0;
    visited_cities[task->ant_tours[offset]] = 1;
    current_city = task->ant_tours[offset];

    for (int k = 1; k < task->num_cities; k++)
    {
        total_probabilities = 0;
        pthread_mutex_lock(&rand_mutex);
        random_num = (double)rand() / RAND_MAX;
        pthread_mutex_unlock(&rand_mutex);
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
            for (int n = task->num_cities - 1; n >= 0; n--)
            {
                if (visited_cities[n] == 0)
                {
                    next_city = n;
                    break;
                }
            }
        }

        offset = ant_id * task->num_cities + k;
        pthread_mutex_lock(&ant_tours_mutex);
        task->ant_tours[offset] = next_city;
        pthread_mutex_unlock(&ant_tours_mutex);

        offset = current_city * task->num_cities + next_city;
        pthread_mutex_lock(&ant_lengths_mutex);
        task->ant_lengths[ant_id] += task->city_distances[offset];
        pthread_mutex_unlock(&ant_lengths_mutex);

        visited_cities[next_city] = 1;
        current_city = next_city;
    }

    // Adding the distance between the starting and ending city to the length of the tour
    offset = ant_id * task->num_cities + (task->num_cities - 1);
    int last_num = task->ant_tours[offset];

    offset = ant_id * task->num_cities + 0;
    int first_num = task->ant_tours[offset];

    offset = last_num * task->num_cities + first_num;
    pthread_mutex_lock(&ant_lengths_mutex);
    task->ant_lengths[ant_id] += task->city_distances[offset];
    pthread_mutex_unlock(&ant_lengths_mutex);

    free(visited_cities);

    return NULL;
}

void update_pheromones(Task *task)
{
    int city1, city2;
    int offset;
    // Evaporating pheromones
    for (int i = 0; i < task->num_cities; i++)
    {
        for (int j = 0; j < task->num_cities; j++)
        {
            offset = i * task->num_cities + j;
            task->pheromones[offset] *= (1.0 - RHO);
        }
    }
    // Depositing pheromones
    for (int i = 0; i < task->num_ants; i++)
    {
        for (int j = 0; j < task->num_cities - 1; j++)
        {

            offset = i * task->num_cities + j;
            city1 = task->ant_tours[offset];
            offset++;
            city2 = task->ant_tours[offset];
            offset = city1 * task->num_cities + city2;
            task->pheromones[offset] += Q / task->ant_lengths[i];
            offset = city2 * task->num_cities + city1;
            task->pheromones[offset] += Q / task->ant_lengths[i];
        }
        offset = i * task->num_cities + task->num_cities - 1;
        city1 = task->ant_tours[offset];
        offset = i * task->num_cities + 0;
        city2 = task->ant_tours[offset];
        offset = city1 * task->num_cities + city2;
        task->pheromones[offset] += Q / task->ant_lengths[i];
        offset = city2 * task->num_cities + city1;
        task->pheromones[offset] += Q / task->ant_lengths[i];
    }
}

void find_best_tour(Task *task, int *best_tour, double *best_length)
{
    int offset;
    for (int i = 0; i < task->num_ants; i++)
    {
        if (task->ant_lengths[i] < *best_length)
        {
            *best_length = task->ant_lengths[i];
            for (int j = 0; j < task->num_cities; j++)
            {
                offset = i * task->num_cities + j;
                best_tour[j] = task->ant_tours[offset];
            }
        }
    }
}