#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <omp.h>

#define ALPHA 1.0
#define BETA 2.0
#define RHO 0.5
#define Q 100.0

void init_distance_matrix(const char *distances, int num_cities, double (*city_distances)[num_cities]);
void init_pheromones(int num_cities, double (*pheromones)[num_cities]);
void init_ants(int num_ants, int num_cities, int (*ant_tours)[num_cities], double *ant_lengths);
void generate_solutions(int num_ants, int num_cities, double (*city_distances)[num_cities], double (*pheromones)[num_cities], int (*ant_tours)[num_cities], double *ant_lengths);
void update_pheromones(int num_cities, int num_ants, double (*pheromones)[num_cities], int (*ant_tours)[num_cities], double *ant_lengths);
void find_best_tour(int num_cities, int num_ants, int (*ant_tours)[num_cities], double *ant_lengths, int *best_tour, double *best_length);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    double total_time;
    clock_t start, end;

    int num_iterations = 100;
    int num_ants;
    int max_ants = 100;
    int num_cities = 312;
    double city_distances[num_cities][num_cities];
    double pheromones[num_cities][num_cities];
    int best_tour[num_cities];
    double best_length;

    FILE *file;
    if ((file = fopen("data/times_usca312.txt", "w")) == NULL)
    {
        printf("File opening error");
        exit(-1);
    }

    init_distance_matrix("data/usca312.txt", num_cities, city_distances);

    for (num_ants = 2; num_ants <= max_ants; num_ants++)
    {
        start = clock();

        omp_set_num_threads(num_ants);
        double ant_lengths[num_ants];
        int ant_tours[num_ants][num_cities];
        best_length = INFINITY;

        init_pheromones(num_cities, pheromones);

        for (int i = 0; i < num_iterations; i++)
        {

            init_ants(num_ants, num_cities, ant_tours, ant_lengths);
            generate_solutions(num_ants, num_cities, city_distances, pheromones, ant_tours, ant_lengths);
            update_pheromones(num_cities, num_ants, pheromones, ant_tours, ant_lengths);
            find_best_tour(num_cities, num_ants, ant_tours, ant_lengths, best_tour, &best_length);
        }

        printf("\nBest tour: ");
        for (int i = 0; i < num_cities; i++)
        {
            printf("%d ", best_tour[i]);
        }
        printf("\nBest tour length: %lf\n", best_length);
        end = clock();
        total_time = ((double)(end - start)) / CLK_TCK;

        fprintf(file, "%d %lf %lf\n", num_ants, total_time, best_length);
    }

    fclose(file);

    return 0;
}

void init_distance_matrix(const char *distances, int num_cities, double (*city_distances)[num_cities])
{
    FILE *fp;
    if ((fp = fopen(distances, "r")) == NULL)
    {
        printf("File opening error");
        exit(-1);
    }

    int i, j;

    for (i = 0; i < num_cities; i++)
    {
        for (j = 0; j < num_cities; j++)
        {
            fscanf(fp, "%lf", city_distances[i] + j);
        }
    }

    fclose(fp);
}

void init_pheromones(int num_cities, double (*pheromones)[num_cities])
{
    for (int i = 0; i < num_cities; i++)
    {
        for (int j = 0; j < num_cities; j++)
        {
            pheromones[i][j] = 100;
        }
    }
}

void init_ants(int num_ants, int num_cities, int (*ant_tours)[num_cities], double *ant_lengths)
{
    for (int i = 0; i < num_ants; i++)
    {
        ant_tours[i][0] = rand() % num_cities;
        for (int j = 1; j < num_cities; j++)
        {
            ant_tours[i][j] = -1;
        }
        ant_lengths[i] = 0;
    }
}

void generate_solutions(int num_ants, int num_cities, double (*city_distances)[num_cities], double (*pheromones)[num_cities], int (*ant_tours)[num_cities], double *ant_lengths)
{
#pragma omp parallel
    {
        double total_probabilities, random_num, probability_sum, probability_to_move;
        int current_city, next_city, next_city_found;
        int *visited_cities;
        visited_cities = (int *)malloc(num_cities * sizeof(int));
        if (visited_cities == NULL)
        {
            printf("Memory not allocated.\n");
            exit(0);
        }

#pragma omp for
        for (int i = 0; i < num_ants; i++)
        {
            for (int j = 0; j < num_cities; j++)
            {
                visited_cities[j] = 0;
            }
            visited_cities[ant_tours[i][0]] = 1;
            current_city = ant_tours[i][0];

            for (int k = 1; k < num_cities; k++)
            {
                total_probabilities = 0;
                random_num = (double)rand() / RAND_MAX;
                probability_sum = 0;
                for (int l = 0; l < num_cities; l++)
                {
                    if (visited_cities[l] == 0)
                    {
                        total_probabilities += pow(pheromones[current_city][l], ALPHA) * pow(1.0 / city_distances[current_city][l], BETA);
                    }
                }

                next_city_found = 0;
                for (int m = 0; m < num_cities; m++)
                {
                    if (visited_cities[m] == 0)
                    {
                        probability_sum += pow(pheromones[current_city][m], ALPHA) * pow(1.0 / city_distances[current_city][m], BETA);
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
                    for (int n = num_cities - 1; n >= 0; n--)
                    {
                        if (visited_cities[n] == 0)
                        {
                            next_city = n;
                            break;
                        }
                    }
                }

                ant_tours[i][k] = next_city;
                ant_lengths[i] += city_distances[current_city][next_city];
                visited_cities[next_city] = 1;
                current_city = next_city;
            }

            // Adding the distance between the starting and ending city to the length of the tour
            ant_lengths[i] += city_distances[ant_tours[i][num_cities - 1]][ant_tours[i][0]];
        }

        free(visited_cities);
    }
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