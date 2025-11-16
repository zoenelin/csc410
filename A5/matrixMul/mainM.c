#include "matrixMul.h"
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);

    // Get the start time
    double start_time = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int processRows = N / size;
    int remainder = N % size;
    int start;
    if (rank < remainder) {
        start = rank * (processRows + 1);
    } else {
        start = rank * processRows + remainder;
    }
    int end;
    if (rank < remainder) {
        end = start + processRows + 1;
    } else {
        end = start + processRows;
    }

    // Dynamically allocate memory for the matrices
    int** A = (int**)malloc(N * sizeof(int*));
    int** B = (int**)malloc(N * sizeof(int*));
    int** C = (int**)malloc(N * sizeof(int*));
    
    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }

    if (A == NULL || B == NULL || C == NULL) {
        if (rank == 0)
            printf("Memory allocation failed!\n");
        MPI_Finalize();
        return -1;
    }

    printf("Matrices allocated successfully.\n");

    // Initialize matrices on rank 0
    if (rank == 0) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                A[i][j] = 1;
                B[i][j] = 1;
                C[i][j] = 0;
            }
        printf("Matrices initialized successfully.\n");
    }

    // Broadcast A and B to all
    for (int i = 0; i < N; i++) {
        MPI_Bcast(A[i], N, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(B[i], N, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Compute local part
    matrixMultiply(A, B, C, N, start, end);

    // Synchronize 
    MPI_Barrier(MPI_COMM_WORLD);

    // Print result from each rank in order
    for (int r = 0; r < size; r++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == r) {
            // printf("Matrix multiplication complete!\n");
            printf("Matrix multiplication complete on rank %d!\n", rank);
            // Optionally display the resulting matrix C
            printf("Resulting Matrix C:\n");
            displayMatrix(C, N);
        }
    }

    // Free dynamically allocated memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    // Get the end time
    double end_time = MPI_Wtime();

    // Calculate and print the elapsed time
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    MPI_Finalize();

    return 0;
}
