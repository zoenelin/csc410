// Parallelize N queens with pthreads!

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define N 15 // Board size
#define T 2 // Number of threads

pthread_mutex_t lock;
int total_solutions = 0;

// check if placing a queen at (row, col) is safe
bool isSafe(int board[], int row, int col) 
{
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == col - i)
            return false;
    }
    return true;
}

// recursive backtracking 
int solve(int board[], int col) 
{
    if (col == N) {
        return 1;
    }
    int count = 0;

    for (int row = 0; row < N; row++) {
        if (isSafe(board, row, col)) {
            board[col] = row;
            count += solve(board, col + 1);
        }
    }
    return count;
}

// thread function that solves its own set of rows
void *threadSolver(void *arg) 
{
    int *rows = (int *)arg; 
    int start = rows[0];
    int end = rows[1];
    free(arg);

    int count = 0;
    for (int row = start; row < end; row++) 
    {
        int board[N];
        for (int i = 0; i < N; i++)
            board[i] = -1;
        board[0] = row;
        count += solve(board, 1);
    }

    pthread_mutex_lock(&lock);
    total_solutions += count;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() 
{
    /*
    int board[N];

    // initializing board ...
    for (int i = 0; i < N; i++)
        board[i] = -1;

    solve(board, 0);
    */
    pthread_t threads[T];
    pthread_mutex_init(&lock, NULL);

    int rowsPerThread = N / T;
    int remainder = N % T;
    int currentRow = 0;

    for (int i = 0; i < T; i++) {
        int extra = (i < remainder);
        int start = currentRow;
        int end = start + rowsPerThread + extra;

        int *range = (int *)malloc(2 * sizeof(int));
        range[0] = start;
        range[1] = end;

        pthread_create(&threads[i], NULL, threadSolver, range);
        currentRow = end;
    }

    // wait for threads
    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    printf("Total solutions for N=%d: %d\n", N, total_solutions);
    return 0;
}
