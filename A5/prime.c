#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

void sieveOfEratosthenes(int n, int rank, int size) {
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; p <= sqrt(n); p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    int start = rank * (n / size) + 1;
    int end   = (rank == size - 1) ? n : (rank + 1) * (n / size);

    bool *localPrime = (bool *)malloc((end - start + 1) * sizeof(bool));
    for (int i = 0; i <= end - start; i++) {
        localPrime[i] = true;
    }

    // note any non-primes
    for (int p = 2; p <= sqrt(n); p++) {
        if (is_prime[p]) {
            int multiple = (start + p - 1) / p * p;
            for (int j = multiple; j <= end; j += p) {
                localPrime[j - start] = false;
            }
        }
    }

    // root prints/gathers
    if (rank == 0) {
        printf("Prime numbers up to %d are:\n", n);

        for (int i = 0; i <= end - start; i++) {
            if (localPrime[i]) {
                printf("%d ", start + i);
            }
        }

        // receive other processes chunks
        for (int r = 1; r < size; r++) {
            int chunkStart = r * (n / size) + 1;
            int chunkEnd;
            if (r == size - 1) {
                chunkEnd = n;
            } 
            else {
                chunkEnd = (r + 1) * (n / size);
            }
            int chunkLength   = chunkEnd - chunkStart + 1;

            bool *chunkPrime = (bool *)malloc(chunkLength * sizeof(bool));

            MPI_Recv(chunkPrime, chunkLength, MPI_C_BOOL, r, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < chunkLength; j++) {
                if (chunkPrime[j]) {
                    printf("%d ", chunkStart + j);
                }
            }

            free(chunkPrime);
        }

        printf("\n");

    } 
    else {
        // all ranks but 0 sends segments
        MPI_Send(localPrime, end - start + 1, MPI_C_BOOL, 0, 0,
                 MPI_COMM_WORLD);
    }

    free(localPrime);
    free(is_prime);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    // Get the start time
    double start_time = MPI_Wtime();

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10000000;
    sieveOfEratosthenes(n, rank, size);

    // Get the end time
    double end_time = MPI_Wtime();

    // Calculate and print the elapsed time
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    MPI_Finalize();
    return 0;
}
