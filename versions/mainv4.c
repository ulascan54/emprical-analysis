#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> // Include the pthread library for threading

// Function prototypes and declarations
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
void quickSortWrapper(int *array, int size);
void improvedQuickSortWrapper(int *array, int size);
void mergeSortWrapper(int *array, int size);
double averageTime(double times[], int count);
void *performSorts(void *args);

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

    for (int i = 0; i < 5; i++)
    {
        copyArray(args->baseArray, array, args->size);
        start = clock();
        args->sortFunction(array, args->size);
        end = clock();
        times[i] = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (i == 0)
        {
            printf("%sIgnored first run for %s%s\n", YELLOW, args->sortName, RESET);
        }
        else
        {
            printf("%s%s run %d time: %f seconds%s\n", GREEN, args->sortName, i, times[i], RESET);
        }
    }
    printf("%s%s average time (excluding first run): %f seconds%s\n", CYAN, args->sortName, averageTime(times, 5), RESET);
    saveArrayToFile(array, args->size, args->fileName);

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

int main()
{
    int size = 100000;
    int *baseArray = malloc(size * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        baseArray[i] = rand() % size;
    }

    saveArrayToFile(baseArray, size, "unsorted_array.txt");

    pthread_t threads[6];
    SortArgs args[6] = {
        {baseArray, size, bubbleSort, "Bubble Sort", "bubble_sorted.txt"},
        {baseArray, size, improvedBubbleSort, "Improved Bubble Sort", "improved_bubble_sorted.txt"},
        {baseArray, size, selectionSort, "Selection Sort", "selection_sorted.txt"},
        {baseArray, size, quickSortWrapper, "Quick Sort", "quick_sorted.txt"},
        {baseArray, size, improvedQuickSortWrapper, "Improved Quick Sort", "improved_quick_sorted.txt"},
        {baseArray, size, mergeSortWrapper, "Merge Sort", "merge_sorted.txt"}};

    // Create threads for each sorting function
    for (int i = 0; i < 6; i++)
    {
        pthread_create(&threads[i], NULL, performSorts, (void *)&args[i]);
    }

    // Join threads to ensure all are completed before exiting main
    for (int i = 0; i < 6; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(baseArray);
    return 0;
}
