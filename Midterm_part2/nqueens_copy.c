// Parallelize N queens with pthreads!

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 15 // Board size

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
void solve(int board[], int col) 
{
    if (col == N) {
        total_solutions++;
        return;
    }

    for (int row = 0; row < N; row++) {
        if (isSafe(board, row, col)) {
            board[col] = row;
            solve(board, col + 1);
        }
    }
}

int main() 
{
    int board[N];

    // initializing board ...
    for (int i = 0; i < N; i++)
        board[i] = -1;

    solve(board, 0);

    printf("Total solutions for N=%d: %d\n", N, total_solutions);
    return 0;
}
