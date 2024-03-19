#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 2
#define NUM_BUCKETS 10
#define NUM_ELEMENTS 1000


void merge(int arr[], int left, int mid, int right);
void merge_sort(int arr[], int left, int right);
void merge_buckets(int buckets[][NUM_ELEMENTS], int arr[]);
void bucket_sort(int arr[]);
void * runner(void * args);
void print_array(int arr[]);
void fill_array(int arr[]);


typedef struct ThreadArgs{
    int (*arr)[NUM_ELEMENTS];
    int left;
    int right;
} thread_args;


int main() {

    clock_t start, end;
    double time_taken;
    
        
    int arr[NUM_ELEMENTS];
        

        
        
    fill_array(arr);

    printf("Unsorted Array:\n");
    print_array(arr);

    start = clock();
    bucket_sort(arr);
    end = clock();
    
    printf("Sorted Array:\n");
    print_array(arr);

    time_taken += ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("time taken: %f seconds\n", time_taken);

    return 0;
}

void merge(int arr[], int left, int mid, int right) {
    int l1 = mid - left + 1;
    int l2 = right - mid;
    int L[l1], R[l2];

    for (int i = 0; i < l1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < l2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;
    while (i < l1 && j < l2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < l1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < l2) {
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

void merge_buckets(int buckets[][NUM_ELEMENTS], int arr[]) {
    int id = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            if (buckets[i][j] != -1) {
                arr[id++] = buckets[i][j];
            }
        }
    }
}

void bucket_sort(int arr[]) {
    int buckets[NUM_BUCKETS][NUM_ELEMENTS];
    pthread_t threads[NUM_THREADS];
    thread_args thread_args[NUM_THREADS];
    int buckets_for_thread = NUM_BUCKETS / NUM_THREADS;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            buckets[i][j] = -1;
        }
    }

   
       for (int i = 0; i < NUM_ELEMENTS; i++) {
        int element = arr[i];
        int bucket_id = element / (100 / NUM_BUCKETS);

        
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            if (buckets[bucket_id][j] == -1) {
                buckets[bucket_id][j] = element;
                break; 
            }
        }
    }

    for(int i=0;i<NUM_THREADS;i++){
        thread_args[i].arr = buckets;
        thread_args[i].left = i * buckets_for_thread;
        thread_args[i].right = (i + 1) * buckets_for_thread;

        
        if (i == NUM_THREADS - 1) {
            thread_args[i].right = NUM_BUCKETS;
        }
        pthread_create(&threads[i], NULL, runner,(void*) &thread_args[i]);
    }
     for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); 
    }
   
    merge_buckets(buckets, arr);
}


void * runner(void * args){
    thread_args * t_args = (thread_args *) args;
    for (int i = t_args->left; i < t_args->right; i++) {
        if (t_args->arr[i][0] != -1) {
            merge_sort(t_args->arr[i], 0, NUM_ELEMENTS - 1);
        }
    }
    pthread_exit(0);
}
void print_array(int arr[]) {
    for (int i = 0; i < NUM_ELEMENTS; i++) {
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