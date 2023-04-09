#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

void crew_prefix(FILE *graphviz, int *X, int n);

int main()
{

    int n = 8;
    int X[n];
    FILE *graphviz;

    crew_prefix(graphviz, X, n);
    printf("Task finished.");

    return 0;
}

void crew_prefix(FILE *graphviz, int *X, int n)
{
    int i, j;
    int logn = (int)log2(n);

    if ((graphviz = fopen("graphviz.txt", "w")) == NULL)
    {
        printf("File opening error");
        exit(-1);
    }

    fprintf(graphviz, "digraph G {\n\n start");

    for (i = 0; i < logn; i++)
    {
#pragma omp parallel for
        for (j = pow(2, i); j < n; j += pow(2, i + 1))
        {
            fprintf(graphviz, " -> \" (%d, %d) \" ", (int)(j - pow(2, i)), (int)(j + pow(2, i)));
        }
    }

    fprintf(graphviz, " -> end \n}\n");

    fclose(graphviz);
}
