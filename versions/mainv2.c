#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b);
void printArray(int array[], int size);
void copyArray(int source[], int dest[], int size);
void bubbleSort(int array[], int size);
void improvedBubbleSort(int array[], int size);
void selectionSort(int array[], int size);
void quickSort(int array[], int low, int high);
int partition(int array[], int low, int high); // Declaration for partition
void improvedQuickSort(int array[], int low, int high);
void mergeSort(int array[], int l, int r);
void merge(int arr[], int l, int m, int r); // Declaration for merge
void saveArrayToFile(int array[], int size, const char *filename);

// ANSI Color Codes
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

// Utility functions
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
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d\n", array[i]);
    }
    fclose(file);
}

// Sorting algorithms
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
        if (swapped == 0)
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
    for (int j = low; j <= high - 1; j++)
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

    int L[n1], R[n2];

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
}

// Main function to test the sorting algorithms
int main()
{
    int size = 100000; // Adjust size as needed for your tests
    int *baseArray = malloc(size * sizeof(int));
    int *array = malloc(size * sizeof(int));

    // Populate baseArray with random numbers
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        baseArray[i] = rand() % size;

    saveArrayToFile(baseArray, size, "unsorted_array.txt");

    // Measure sorting times and save sorted arrays
    clock_t start, end;
    double cpu_time_used;

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            printf("Ignored run %d\n", i + 1);
        }
        if (i != 0)
        {
            printf("Iteration %d\n", i + 1);
        }

        printf(GREEN "Testing Bubble Sort...\n" RESET);
        copyArray(baseArray, array, size);
        start = clock();
        bubbleSort(array, size);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(GREEN "Bubble Sort took %f seconds to execute \n" RESET, cpu_time_used);
        saveArrayToFile(array, size, "bubble_sorted.txt");

        printf(YELLOW "Testing Improved Bubble Sort...\n" RESET);
        copyArray(baseArray, array, size);
        start = clock();
        improvedBubbleSort(array, size);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(YELLOW "Improved Bubble Sort took %f seconds to execute \n" RESET, cpu_time_used);
        saveArrayToFile(array, size, "improved_bubble_sorted.txt");

        printf(BLUE "Testing Selection Sort...\n" RESET);
        copyArray(baseArray, array, size);
        start = clock();
        selectionSort(array, size);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(BLUE "Selection Sort took %f seconds to execute \n" RESET, cpu_time_used);
        saveArrayToFile(array, size, "selection_sorted.txt");

        printf(RED "Testing Quick Sort...\n" RESET);
        copyArray(baseArray, array, size);
        start = clock();
        quickSort(array, 0, size - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(RED "Quick Sort took %f seconds to execute \n" RESET, cpu_time_used);
        saveArrayToFile(array, size, "quick_sorted.txt");

        printf(MAGENTA "Testing Improved Quick Sort...\n" RESET);
        copyArray(baseArray, array, size);
        start = clock();
        improvedQuickSort(array, 0, size - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(MAGENTA "Improved Quick Sort took %f seconds to execute \n" RESET, cpu_time_used);
        saveArrayToFile(array, size, "improved_quick_sorted.txt");

        printf(CYAN "Testing Merge Sort...\n" RESET);
        copyArray(baseArray, array, size);
        start = clock();
        mergeSort(array, 0, size - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(CYAN "Merge Sort took %f seconds to execute \n" RESET, cpu_time_used);
        saveArrayToFile(array, size, "merge_sorted.txt");
    }

    free(baseArray);
    free(array);
    return 0;
}
