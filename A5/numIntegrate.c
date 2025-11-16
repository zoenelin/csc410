#include <stdio.h>
#include <mpi.h>

#define N 1000000000 // 1 billion intervals

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

double trapezoidalRule(double a, double b, long long start, long long end, double h) {
    double sum = 0.0;
    for (long long i = start; i < end; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    // Get the start time
    double start_time = MPI_Wtime();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 1.0;
    double h = (b - a) / N;

    long long intervalsPerProcess = N / size;
    long long start = rank * intervalsPerProcess;
    long long end ;
    if (rank == size - 1) {
        end = N;
    } else {
        end = start + intervalsPerProcess;
    }

    // processes compute local sum
    double localSum = trapezoidalRule(a, b, start, end, h);

    // local sums of processes reduced on rank 0 (global sum)
    double globalSum = 0.0;
    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        globalSum += 0.5 * (f(a) + f(b)) * h;
        printf("Estimated value of π: %.15f\n", globalSum);
    }

    // Get the end time
    double end_time = MPI_Wtime();

    // Calculate and print the elapsed time
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    MPI_Finalize();
    return 0;
}
