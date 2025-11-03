#include "nqueens.h"

int main() 
{
    /*
    int n = 8;
    int* board = (int*)malloc(n * sizeof(int));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        board[i] = -1;
    }

    solveNQueensUtil(board, 0, n);

    free(board);
    return 0;
    */

    omp_set_num_threads(7);

    int n = 15;

    // Start timing
    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(guided)
    for (int row = 0; row < n; row++) {
        int* board = (int*)malloc(n * sizeof(int));
        if (board == NULL) continue;

        for (int i = 0; i < n; i++) board[i] = -1;
        board[0] = row;

        solveNQueensUtil(board, 1, n);

        free(board);
    }

    // End timing
    double end_time = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_time - start_time);

    return 0;
}
