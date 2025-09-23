#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool isSafe(int board[], int row, int col, int n) 
{
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

int solveNQueensUtil(int board[], int col, int n) 
{
    // Base case
    if (col == n) return 1;

    // Recursive case
    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[col] = i;
            if (solveNQueensUtil(board, col + 1, n)) {
                return 1;
            }
            board[col] = -1;
        }
    }
    return 0;
}

/*
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
*/
