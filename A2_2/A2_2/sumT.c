#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 100000000
#define NUM_THREADS 4

long long arr[SIZE];
//int partialSums[NUM_THREADS] = {0}; // Array to store partial sums for each thread

// ^^ If we use int, wouldn't this cause an overflow? ^^
// when using int i got Total Sum: -3307507584 and when using long long i got Total Sum: 5000000050000000 (which is the expected output)
// To me it appears that its wrapping due to the overflow, let me know if I was wrong to change the type from int to long long

long long partialSums[NUM_THREADS] = {0}; 

// Entry function for each thread
void* sumPart(void* arg) 
{
    // Divide the work for each thread based on their id and let them compute partial sums
        // -----> Write your code here
        int thread_id = *((int*)arg);
        long long start = (SIZE / NUM_THREADS) * thread_id;
        long long end = (SIZE / NUM_THREADS) * (thread_id + 1);

        for (long long i = start; i < end; i++) {
            partialSums[thread_id] += arr[i];
        }

        pthread_exit(0);
}

int main() 
{
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads to compute partial sums
        // ------> Write your code here
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumPart, (void*)&thread_ids[i]);
    }

    // Wait for all threads to finish
        // -------> Write your code here
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combine the partial sums from all threads
    long long totalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalSum += partialSums[i];
    }

    // Print the total sum;
    printf("Total Sum: %lld\n", totalSum);

    return 0;
}
