#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

// Function declarations
void swap(int *a, int *b);
void printArray(int array[], int size);
void copyArray(int source[], int dest[], int size);
void bubbleSort(int array[], int size);
void improvedBubbleSort(int array[], int size);
void selectionSort(int array[], int size);
void quickSort(int array[], int low, int high);
int partition(int array[], int low, int high);
void improvedQuickSort(int array[], int low, int high);
void mergeSort(int array[], int l, int r);
void merge(int arr[], int l, int m, int r);
void saveArrayToFile(int array[], int size, const char *filename);
double averageTime(double times[], int count);
void *performSorts(void *args);
void quickSortWrapper(int *array, int size);
void improvedQuickSortWrapper(int *array, int size);
void mergeSortWrapper(int *array, int size);

// ANSI Color Codes
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

typedef struct
{
    int *baseArray;
    int size;
    void (*sortFunction)(int *, int);
    char *sortName;
    char *fileName;
} SortArgs;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int array[], int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void copyArray(int source[], int dest[], int size)
{
    for (int i = 0; i < size; i++)
        dest[i] = source[i];
}

void saveArrayToFile(int array[], int size, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d\n", array[i]);
    }
    fclose(file);
}

double averageTime(double times[], int count)
{
    double sum = 0;
    for (int i = 1; i < count; i++)
    {
        sum += times[i];
    }
    return sum / (count - 1); // Exclude the first run
}
void *performSorts(void *arguments)
{
    SortArgs *args = (SortArgs *)arguments;
    int *array = malloc(args->size * sizeof(int));
    double times[5];
    clock_t start, end;
    FILE *logFile;

    char logFilename[256];
    snprintf(logFilename, sizeof(logFilename), "outputs/%s_times.txt", args->fileName); // Create filename for log
    logFile = fopen(logFilename, "a");                                                  // Open in append mode

    char sortedFilename[256];
    snprintf(sortedFilename, sizeof(sortedFilename), "sorted/%s_%d.txt", args->fileName, args->size); // Path for sorted array

    for (int i = 0; i < 5; i++)
    {
        copyArray(args->baseArray, array, args->size);
        start = clock();
        args->sortFunction(array, args->size);
        end = clock();
        times[i] = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(logFile, "Run %d: %f seconds\n", i + 1, times[i]);
        printf("%s%s - Run %d: %f seconds%s\n", BLUE, args->sortName, i + 1, times[i], RESET);
    }
    double avgTime = averageTime(times, 5);
    fprintf(logFile, "for %d size, Average time (excluding first run): %f seconds\n", args->size, avgTime);
    printf("%s for %d size,  Average time (excluding first run) for %s: %f seconds%s\n", GREEN, args->size, args->sortName, avgTime, RESET);

    fclose(logFile);

    saveArrayToFile(array, args->size, sortedFilename); // Save the sorted array to the sorted directory
    free(array);
    return NULL;
}

// Wrapper functions for the sorting algorithms that require three parameters
void quickSortWrapper(int *array, int size)
{
    quickSort(array, 0, size - 1);
}

void improvedQuickSortWrapper(int *array, int size)
{
    improvedQuickSort(array, 0, size - 1);
}

void mergeSortWrapper(int *array, int size)
{
    mergeSort(array, 0, size - 1);
}

void bubbleSort(int array[], int size)
{
    int i, j;
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - i - 1; j++)
            if (array[j] > array[j + 1])
                swap(&array[j], &array[j + 1]);
}

void improvedBubbleSort(int array[], int size)
{
    int i, j, swapped;
    for (i = 0; i < size - 1; i++)
    {
        swapped = 0;
        for (j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                swap(&array[j], &array[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }
}

void selectionSort(int array[], int size)
{
    int i, j, min_idx;
    for (i = 0; i < size - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < size; j++)
            if (array[j] < array[min_idx])
                min_idx = j;
        swap(&array[min_idx], &array[i]);
    }
}

void quickSort(int array[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

int partition(int array[], int low, int high)
{
    int pivot = array[high];
    int i = (low - 1);
    for (int j = low; j < high; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void improvedQuickSort(int array[], int low, int high)
{
    if (high - low < 20)
        selectionSort(&array[low], high - low + 1);
    else
    {
        if (low < high)
        {
            int pi = partition(array, low, high);
            improvedQuickSort(array, low, pi - 1);
            improvedQuickSort(array, pi + 1, high);
        }
    }
}

void mergeSort(int array[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(array, l, m);
        mergeSort(array, m + 1, r);
        merge(array, l, m, r);
    }
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void runSortingTests(int *fullArray, int size)
{
    int *baseArray = malloc(size * sizeof(int));
    memcpy(baseArray, fullArray, size * sizeof(int)); // Copy only the needed portion

    char baseFilename[256];
    snprintf(baseFilename, sizeof(baseFilename), "arrays/base_array_%d.txt", size);
    saveArrayToFile(baseArray, size, baseFilename); // Save the base array with size label

    pthread_t threads[6];
    SortArgs args[6] = {
        {baseArray, size, bubbleSort, "Bubble Sort", "bubble_sorted"},
        {baseArray, size, improvedBubbleSort, "Improved Bubble Sort", "improved_bubble_sorted"},
        {baseArray, size, selectionSort, "Selection Sort", "selection_sorted"},
        {baseArray, size, quickSortWrapper, "Quick Sort", "quick_sorted"},
        {baseArray, size, improvedQuickSortWrapper, "Improved Quick Sort", "improved_quick_sorted"},
        {baseArray, size, mergeSortWrapper, "Merge Sort", "merge_sorted"}};

    for (int i = 0; i < 6; i++)
    {
        pthread_create(&threads[i], NULL, performSorts, (void *)&args[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(baseArray);
}

int main()
{
    // const int max_size = 10000000; //! -> for report testing
    const int max_size = 10000000;
    int *fullArray = malloc(max_size * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < max_size; i++)
    {
        fullArray[i] = rand() % max_size; // Generate the full array only once
    }

    int sizes[] = {100, 1000, 10000, 100000, 1000000, 5000000, 10000000};
    // int sizes[] = {100, 1000, 10000, 100000, 1000000, 5000000, 10000000}; //! -> for report testing
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++)
    {
        printf("%sRunning sorting tests for size: %d%s\n", CYAN, sizes[i], RESET);
        runSortingTests(fullArray, sizes[i]); // Pass only the needed portion
    }

    free(fullArray);
    return 0;
}
