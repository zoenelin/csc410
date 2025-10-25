#include "p_merge.h"

void merge(int* arr, int l, int m, int r) 
{
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort_seq(int* arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// recursive thread creation for merge sort
void* parallel_merge_sort(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int l = data->left;
    int r = data->right;
    int level = data->level;

    if (l >= r) {
        pthread_exit(NULL);
    }
    int m = (l + r) / 2;

    // prevent thread explosion
    if (level >= MAX_LEVEL) {
        merge_sort_seq(data->arr, l, r);
        pthread_exit(NULL);
    }
    pthread_t left_thread, right_thread;
    thread_data_t left_data = {data->arr, l, m, level + 1};
    thread_data_t right_data = {data->arr, m + 1, r, level + 1};

    pthread_create(&left_thread, NULL, parallel_merge_sort, (void*)&left_data);
    pthread_create(&right_thread, NULL, parallel_merge_sort, (void*)&right_data);

    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);

    merge(data->arr, l, m, r);
    pthread_exit(NULL);
}

// starting point for recursive parallel merge sort
void merge_sort_p(int* arr, int l, int r) 
{
    thread_data_t data = {arr, l, r, 0};
    parallel_merge_sort(&data);
}


void printArray(int* arr, int n) 
{
//    for (int i = 0; i < n; i++)
//        printf("%d ", arr[i]);
//    printf("\n");
}

double time_diff_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}