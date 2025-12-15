%%writefile mainMOpenCL.c
#include "matrixMulOpenCL.h"

const char* kernelSource =
"__kernel void matrixMul(__global int* A, __global int* B, __global int* C, const int N) {\n"
"   int row = get_global_id(0);\n"
"   int col = get_global_id(1);\n"
"   if (row < N && col < N) {\n"
"       int sum = 0;\n"
"       for (int k = 0; k < N; k++) {\n"
"           sum += A[row * N + k] * B[k * N + col];\n"
"       }\n"
"       C[row * N + col] = sum;\n"
"   }\n"
"}\n";

int main()
{

/*
    // Dynamically allocate memory for the matrices
    int** A = (int**)malloc(N * sizeof(int*));
    int** B = (int**)malloc(N * sizeof(int*));
    int** C = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }
*/

    int* A = (int*)malloc(N * N * sizeof(int));
    int* B = (int*)malloc(N * N * sizeof(int));
    int* C = (int*)malloc(N * N * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    printf("Matrices allocated successfully.\n");

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = 1;
            B[i * N + j] = 1;
            C[i * N + j] = 0;
        }
    }

/*
    // Initialize matrices A and B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }
*/

    // OpenCL matrix multiplication
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem bufferA, bufferB, bufferC;
    cl_int ret;

    ret = clGetPlatformIDs(1, &platform_id, NULL);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "matrixMul", &ret);

    // Create buffers
    bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, N * N * sizeof(int), NULL, &ret);
    bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, N * N * sizeof(int), NULL, &ret);
    bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, N * N * sizeof(int), NULL, &ret);

    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, N * N * sizeof(int), A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, N * N * sizeof(int), B, 0, NULL, NULL);

    // Set kernel arguments
    int Nval = N; // value required for kernel argument

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    clSetKernelArg(kernel, 3, sizeof(int), &Nval);

    size_t globalSize[2] = {N, N};

    cl_event event;
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, NULL, 0, NULL, &event);
    clWaitForEvents(1, &event);

    // Timing
    cl_ulong start, end;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
    double time = (end - start) * 1e-6;
    printf("Time: %f ms\n", time);

    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, N * N * sizeof(int), C, 0, NULL, NULL);

    // cleanup OpenCL resources
    clReleaseEvent(event);
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(A);
    free(B);
    free(C);

/*

    printf("Matrices initialized successfully.\n");

    matrixMultiply(A, B, C, N);

    printf("Matrix multiplication complete!\n");

    // Optionally display the resulting matrix C
    //printf("Resulting Matrix C:\n");
    //displayMatrix(C, N);

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

*/

    return 0;
}
