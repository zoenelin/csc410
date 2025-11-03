#include <stdio.h>
#include <omp.h>
#include <stdlib.h> // for malloc

#define SIZE 1000000000

long long sumArray(long long arr[], long long size) 
{
    // Write your code here
    long long sum = 0;

    // Start timing
    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(guided) reduction(+:sum)
    for (long long i = 0; i < size; i++) {
        sum += arr[i];
    }

    // End timing
    double end_time = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_time - start_time);

    return sum;
}

int main() 
{
    omp_set_num_threads(9); // Thread count 

    long long* arr = (long long*)malloc(SIZE * sizeof(long long)); // Using malloc for large array
    for (long long i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    long long totalSum = sumArray(arr, SIZE);
    printf("Total Sum: %lld\n", totalSum);

    return 0;
}
