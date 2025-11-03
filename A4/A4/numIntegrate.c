#include <stdio.h>
#include <omp.h>

#define N 1000000000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() 
{
    // complete this
    double a = 0.0;
    double b = 1.0;
    double h = (b - a) / N;
    double sum = 0.5 * (f(a) + f(b));

    // Start timing
    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(guided) reduction(+:sum)
    for (int i = 1; i < N; i++) {
        double x = a + i * h;
        sum += f(x);
    }

    // End timing
    double end_time = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_time - start_time);

    return sum * h;
}

int main() {
    #pragma omp_set_num_threads(7); // Thread count
    double pi = trapezoidalRule();
    printf("Estimated value of π: %f\n", pi);
    return 0;
}
