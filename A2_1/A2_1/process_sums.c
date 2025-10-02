// convert sequential sums to parallel

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROCESSES 4 

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int *arr = malloc(N * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    // complete this
    int splitChunk = N / NUM_PROCESSES;
    int extra = N % NUM_PROCESSES;
    int pipes[NUM_PROCESSES][2];
    pid_t pid[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            _exit(1);
        }

        pid[i] = fork();
        if (pid[i] < 0) {
            perror("fork");
            _exit(1);
        } 
        
        if (pid[i] == 0) {
            // Child process
            close(pipes[i][0]);

            int start = i * splitChunk;
            int end = start + splitChunk;
            if (i == NUM_PROCESSES - 1) {
                end += extra;
            }

            long partialSum = 0;

            for (int j = start; j < end; j++) {
                partialSum += arr[j];
            }

            write(pipes[i][1], &partialSum, sizeof(partialSum));
            close(pipes[i][1]);
            _exit(0);
        }

        // Parent process
        close(pipes[i][1]);
    }

    long total = 0;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        long partialSum;
        read(pipes[i][0], &partialSum, sizeof(partialSum));
        close(pipes[i][0]);
        total += partialSum;
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pid[i], NULL, 0);
    }

    printf("%ld\n", total);

    return 0;
}
