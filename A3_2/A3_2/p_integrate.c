#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000000 // intervals
#define NUM_THREADS 4

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// use this shared global variable
double total_sum = 0.0;
pthread_mutex_t mutex;

void *parallel_trapezoidalRule(void *arg) 
{
   long threadID = (long)arg;
   double h = 1.0 / N;
   long portionSize = N / NUM_THREADS;
   long start = threadID * portionSize;
   long end;
   if (threadID == NUM_THREADS - 1) {
       end = N;
   } else {
       end = start + portionSize;
   }
   double sum = 0.0;
   // midpoint rule applied to the assigned portion
   for (long i = start; i < end; i++) {
       double midpoint = h * (i + 0.5);
       sum += f(midpoint); // at midpoint of each subinterval add f(midpoint)
   }
   sum *= h;

    pthread_mutex_lock(&mutex);
    total_sum += sum;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, parallel_trapezoidalRule, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);


    //printf("Result of numerical integration: %f\n", total_sum);

    return 0;
}
