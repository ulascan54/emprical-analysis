#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    snprintf(baseFilename, sizeof(baseFilename), "two/two_base_array_%d.txt", size);
    saveArrayToFile(baseArray, size, baseFilename); // Save the base array with size label

    SortArgs args[] = {
        {baseArray, size, bubbleSort, "Bubble Sort", "bubble_sorted"}};
    //* 30 000
    //!{baseArray, size, improvedBubbleSort, "Improved Bubble Sort", "improved_bubble_sorted"}};
    //* 30 000
    //!{baseArray, size, selectionSort, "Selection Sort", "selection_sorted"}};
    //* 56 000
    //!{baseArray, size, quickSortWrapper, "Quick Sort", "quick_sorted"}};
    //* 14 000
    //!{baseArray, size, improvedQuickSortWrapper, "Improved Quick Sort", "improved_quick_sorted"}};
    //* 14 000 000
    //!{baseArray, size, mergeSortWrapper, "Merge Sort", "merge_sorted"}};
    //* 8 000 000

    for (int i = 0; i < 6; i++)
    {
        int *array = malloc(args[i].size * sizeof(int));
        double times[5];
        clock_t start, end;
        FILE *logFile;

        char logFilename[256];
        snprintf(logFilename, sizeof(logFilename), "two/two_%s_times.txt", args[i].fileName); // Create filename for log
        logFile = fopen(logFilename, "a");                                                    // Open in append mode

        char sortedFilename[256];
        snprintf(sortedFilename, sizeof(sortedFilename), "two/two_%s_%d.txt", args[i].fileName, args[i].size); // Path for sorted array

        for (int j = 0; j < 5; j++)
        {
            copyArray(args[i].baseArray, array, args[i].size);
            start = clock();
            args[i].sortFunction(array, args[i].size);
            end = clock();
            times[j] = ((double)(end - start)) / CLOCKS_PER_SEC;
            fprintf(logFile, "Run %d: %f seconds\n", j + 1, times[j]);
            printf("%s%s - Run %d: %f seconds%s\n", BLUE, args[i].sortName, j + 1, times[j], RESET);
        }
        double avgTime = averageTime(times, 5);
        fprintf(logFile, "Size : %d, Average time (excluding first run): %f seconds\n", args[i].size, avgTime);
        printf("%s for %d size,  Average time (excluding first run) for %s: %f seconds%s\n", GREEN, args[i].size, args[i].sortName, avgTime, RESET);

        fclose(logFile);

        saveArrayToFile(array, args[i].size, sortedFilename); // Save the sorted array to the sorted directory
        free(array);
    }

    free(baseArray);
}

#define TOTAL_INPUTS 100000
#define OUTPUT_LIMIT 30300
// 30380

int main()
{
    FILE *inputFile, *outputFile;
    int *inputArray;
    int i;

    // Bellekte yer ayırma
    inputArray = (int *)malloc(TOTAL_INPUTS * sizeof(int));
    if (inputArray == NULL)
    {
        fprintf(stderr, "Bellek tahsisi başarısız!\n");
        return 1;
    }

    // Giriş dosyasını açma
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL)
    {
        fprintf(stderr, "Giriş dosyası açılamadı!\n");
        free(inputArray);
        return 1;
    }

    // Verileri dosyadan okuma
    for (i = 0; i < TOTAL_INPUTS; i++)
    {
        if (fscanf(inputFile, "%d", &inputArray[i]) != 1)
        {
            fprintf(stderr, "Dosya okuma hatası!\n");
            fclose(inputFile);
            free(inputArray);
            return 1;
        }
    }
    fclose(inputFile);

    int sizes[] = {OUTPUT_LIMIT};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++)
    {
        printf("%sRunning sorting tests for size: %d%s\n", CYAN, sizes[i], RESET);
        runSortingTests(inputArray, sizes[i]); // Pass only the needed portion
    }

    free(inputArray);
    return 0;
}
