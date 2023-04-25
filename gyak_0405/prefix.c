#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void crew_prefix(int* A, int n);
int *erew_prefix(int *X, int*Y, int n);

int main() {
    int i, n = 8;
    int A[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int B[n];
    erew_prefix(A, B, n);
    for (i = 0; i < n; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");
    crew_prefix(A, n);
    for (i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    return 0;
}

void crew_prefix(int* A, int n) {
    int i, j;
    int logn = (int)log2(n);
    for (i = 0; i < logn; i++) {
        #pragma omp parallel for
        for (j = (int)pow(2, i); j < n; j += (int)pow(2, i + 1)) {
            A[j] += A[(int)(j - pow(2, i))];
        }
    }
}

int *erew_prefix(int *X, int*Y, int n){
    int i, k = 2;
    Y[0] = X[0];
    #pragma omp parallel
    {
    #pragma omp for
    for (i = 1; i < n; i++)
    {
        Y[i] = X[i-1] + X[i];
    }
    while (k < n)
    {
        #pragma omp for
        for (i = k + 1; i < n; i++)
        {
            Y[i] = Y[i-k] + Y[i];
        }
        k = 2 * k;
    }
    }
    return Y;
}
