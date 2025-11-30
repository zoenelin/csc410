%%writefile matrixMulOpenCL.h
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h> // For malloc() and free()

#define N 1000 // matrix size

void matrixMultiplyOpenCL(int** A, int** B, int** C, int n);

void displayMatrix(int** matrix, int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int** A, int** B, int** C, int n)
{
    // complete this
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
"""

with open("matrixMulOpenCL.h", "w") as f:
    f.write(file_content)

print("matrixMulOpenCL.h written successfully.")