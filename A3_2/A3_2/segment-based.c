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

// divides out segments to threads and sorts segments in parallel
void* parallel_merge_sort(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    merge_sort_seq(data->arr, data->left, data->right);
    pthread_exit(NULL);
}

// here is where the merging of sorted segments happens and parallel sort happens
void merge_sort_p(int* arr, int l, int r) 
{
    int portionSize = (r - l + 1) / NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    // sorting of segments
    for (int i = 0; i < NUM_THREADS; i++) {
        int start = l + i * portionSize;
        int end = (i == NUM_THREADS - 1) ? r : start + portionSize - 1;
        thread_data[i] = (thread_data_t){arr, start, end, 0};
        pthread_create(&threads[i], NULL, parallel_merge_sort, (void*)&thread_data[i]);
    }
    // wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // merge the sorted segments
    int currentSize = portionSize;
    while (currentSize < r - l + 1) {
        for (int i = l; i < r; i += 2 * currentSize) {
            int mid = i + currentSize - 1;
            int rightEnd = (i + 2 * currentSize - 1 > r) ? r : i + 2 * currentSize - 1;
            if (mid < rightEnd) {
                merge(arr, i, mid, rightEnd);
            }
        }
        currentSize *= 2;
    }
}


void printArray(int* arr, int n) 
{
//    for (int i = 0; i < n; i++)
//       printf("%d ", arr[i]);
//    printf("\n");
}


double time_diff_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}