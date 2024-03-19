#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_BUCKETS 10
#define NUM_ELEMENTS 1000

void merge(int arr[], int left, int mid, int right);
void merge_sort(int arr[], int left, int right);
void merge_buckets(int buckets[][NUM_ELEMENTS], int arr[], int n);
void bucket_sort(int arr[]);
void * runner(void * args);
void print_array(int arr[], int n);
void fill_array(int arr[]);

int main() {
    clock_t start, end;
    double total_time;
    int arr[NUM_ELEMENTS];
    
    
    fill_array(arr);
    
    printf("Unsorted Array:\n");
    print_array(arr, NUM_ELEMENTS);

    start = clock();
    bucket_sort(arr);
    end = clock();

    printf("Sorted Array:\n");
    print_array(arr, NUM_ELEMENTS);
    
    total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", total_time);

    return 0;
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void merge_buckets(int buckets[][NUM_ELEMENTS], int arr[], int n) {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            if (buckets[i][j] != -1) {
                arr[index++] = buckets[i][j];
            }
        }
    }
}

void bucket_sort(int arr[]) {
    int buckets[NUM_BUCKETS][NUM_ELEMENTS];
    int bucket_counts[NUM_BUCKETS] = {0};


    #pragma omp parallel for collapse(2)
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            buckets[i][j] = -1;
        }
    }


    #pragma omp parallel for
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int bucket_index = arr[i] / (100 / NUM_BUCKETS);
        
       
        int pos;
    #pragma omp atomic capture
        {
            pos = bucket_counts[bucket_index];
            bucket_counts[bucket_index]++;
        }

        
        buckets[bucket_index][pos] = arr[i];
    }


    
    #pragma omp parallel for
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (bucket_counts[i] > 0) {
            merge_sort(buckets[i], 0, bucket_counts[i] - 1);
        }
    }

    
    merge_buckets(buckets, arr, NUM_ELEMENTS);
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
void fill_array(int arr[]){
    srand(time(NULL));
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        arr[i] = rand() % 100;
    }
}

