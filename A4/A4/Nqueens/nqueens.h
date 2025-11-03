#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

bool isSafe(int board[], int row, int col, int n) 
{
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

void printBoard(int board[], int n) 
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[j] == i) {
                printf("Q");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void solveNQueensUtil(int board[], int col, int n) 
{
    // Base case
        // write your code here
    if (col == n) {
    //    #pragma omp critical
    //    printBoard(board, n);
        return;
    }

    // Recursive case
        // write your code here
    for (int row = 0; row < n; row++) {
        if (isSafe(board, row, col, n)) {
            board[col] = row;
            solveNQueensUtil(board, col + 1, n);
            board[col] = -1;
        }
    }
}



