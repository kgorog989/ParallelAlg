#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void fill_array(int *array, int lower, int upper, int size);
void crew_prefix(FILE *graphviz, int processors, int *X, int n);
void print_array(int *array, int size);

int main()
{

	int n = 8, lower = 1, upper = 6;
    int X[n];
    int processors = 2;
	FILE *graphviz;

    fill_array(X, lower, upper, n);
    print_array(X, n);
    crew_prefix(graphviz, processors, X, n);
    print_array(X, n);

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

void crew_prefix(FILE *graphviz, int processors, int *X, int n)
{
    int i, j, k;
    
    if((graphviz = fopen("graphviz.txt", "w")) == NULL)
    {
        printf("File opening error");
        exit(-1);
    }

    fprintf(graphviz, "digraph G {\n\n");

    for(k = 0; k < processors; k++)
    {
        for (i = 1; i < n; i++)
        {
            for (j = 0; j < floor(log(n)-1); j++)
            {
                if (i - pow(2, j) >= 0)
                {
                    X[i] = X[i] + X[(int)(i - pow(2, j))];
                }
                
            }
            
        }
        
    }



    fprintf(graphviz, "\n}\n");

    fclose(graphviz);
}

void print_array(int *array, int size)
{
    int i;
    
    for (i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
