#include <stdio.h>
#include <stdlib.h> // For malloc() and free()
#include <omp.h>

#define N 1000 // Adjust this to test larger matrix sizes

void displayMatrix(int** matrix, int n) 
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int** A, int** B, int** C, int n) 
{
    // Start timing
    double start_time = omp_get_wtime();

    // complete this
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    // End timing
    double end_time = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_time - start_time);
}