/**
 * Pure C 2019.
 * File: ex3b.
 *
 * Authors: Tasneem Abu Odeh & Anwar Hamdan.
 * id: 314902685, 319109609
 * login: tasnimab + anwarha.
 *
 * A program that build an array that has 500 cells, fill it with random
 * numbers between 0 and 100, then it sorts the array using quick sort and
 * merge sort. After all it prints  out the values (sorted).
 *
 *
 *
 * Input: Nothing.
 * output: Sorted array that has 500 values between 0 and 1000 .
 *
 * Compile: gcc -Wall ex3b.c –o ex3b.
 * Run: ex3b.
 * RunTime: O(n^2), while n is the input size.
 *
 */

//include(s) section
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
//#include <wait.h>
#include <stdbool.h>

//define(s) section
#define SIZE_ARR 500
#define NUMBER 17
#define RANGE 1001
#define CHILDS_SIZE 5
#define PIECE_ARR 100


//prototype(s) section
void errorMessage(void);
void sonProcess(int pipe[], int source, int arr[]);
void fatherProcess(int pipeDescs[], const int source, int arr[]);
void QuickSort(int arr[], int source, int target);
void SWAP(int *val1, int *val2);
int partition(int arr[], int source, int target);
void checkPIPE(int pipeDescs[]);
int findMin(int arr[]);
int findMax(int arr[]);
void printSortedArr(int arr[]);
void MergeSort(int arr[], int l, int m, int r);

//main section
int main() {
    
    int index, curr, pipeDescs[2];
    pid_t status;
    int source;
    checkPIPE(pipeDescs);
    srand(NUMBER);
    int arr[SIZE_ARR];
    for (index = 0; index < SIZE_ARR; ++index) {
        arr[index] = (int) (srand(17)); //% RANGE);
    }
    
    for (curr = 0; curr < CHILDS_SIZE; curr++) {
        source = curr * PIECE_ARR;
        status = fork();
        switch (status) {
            case -1:
                errorMessage();
                break;
                
            case 0:
                sonProcess(pipeDescs, source, arr);
                exit(EXIT_SUCCESS);
                break;
        }
    }
    int i = 0;
    for (i = 0; i < 5; i++)
        wait(NULL);
    
    fatherProcess(pipeDescs, source, arr);
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------
//A function that prints out an error message in case it couldn't allocate child process.
void errorMessage() {
    perror("The allocation of the child failed");
    exit(EXIT_FAILURE);
}

//--------------------------------------------------
//A function that do the son process.
void sonProcess(int pipe[], int source, int arr[]) {
    
    const int target = source + PIECE_ARR;
    close(pipe[0]);
    //close(STDOUT_FILENO);
    QuickSort(arr, source, target);
    //dup2(pipe[1],STDOUT_FILENO);
    if (write(pipe[1], &arr[source], sizeof(int) * 100) == -1) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }
    
    
}

//--------------------------------------------------
//A function that do the father process.
void fatherProcess(int pipeDescs[], const int source, int arr[]) {
    close(pipeDescs[1]);
    
    
    if (read(pipeDescs[0], &arr[0], sizeof(int) * 500) == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    
    MergeSort(arr,0,99,199);
    MergeSort(arr,0,199,299);
    MergeSort(arr,0,299,399);
    MergeSort(arr,0,399,499);
    MergeSort(arr,0,499,499);
    
    printSortedArr(arr);
    
    close(pipeDescs[1]);
    
}

//--------------------------------------------------
//A function that sorts the array by using quick sort.
void QuickSort(int arr[], int source, int target) {
    int pivotPlace = partition(arr, source, target);
    
    if (source < pivotPlace - 1)
        QuickSort(arr, source, pivotPlace - 1);
    
    if (target > pivotPlace + 1)
        QuickSort(arr, pivotPlace + 1, target);
}

//--------------------------------------------------
//A function that does the partition.
int partition(int arr[], int source, int target) {
    int i;
    int pivotPlace = source;
    
    for (i = source + 1; i <= target; i++)
        if (arr[i] < arr[pivotPlace]) {
            SWAP(&arr[i], &arr[pivotPlace + 1]);
            SWAP(&arr[pivotPlace + 1], &arr[pivotPlace]);
            pivotPlace++;
        }
    return pivotPlace;
}

//--------------------------------------------------
//A function that swaps between two values.
void SWAP(int *val1, int *val2) {
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}

//--------------------------------------------------
//A function that finds the minimum value.
int findMin(int arr[]) {
    
    int index;
    int min = 0;
    for (index = 0; index < (int) SIZE_ARR; index++)
        if (min > arr[index])
            min = arr[index];
    return min;
    
}

//--------------------------------------------------
//A function that finds the maximum value.
int findMax(int arr[]) {
    
    int index;
    int max = 0;
    for (index = 0; index < (int) SIZE_ARR; index++)
        if (max < arr[index])
            max = arr[index];
    return max;
    
}

//--------------------------------------------------
//check if the pipe is valid.
void checkPIPE(int pipeDescs[]) {
    if (pipe(pipeDescs) == -1) {
        perror("Cannot Pipe:");
        exit(EXIT_FAILURE);
        
    }
}

//--------------------------------------------------
//A function that prints out the array after it was sorted.
void printSortedArr(int arr[]) {
    
    printf("%d ", arr[0]);
    printf("%d", arr[499]);
    puts("\n");
}

//--------------------------------------------------
//A function that sorts the array using MergeSort sort.
void MergeSort(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int Left[n1], Right[n2];
    
    for (i = 0; i < n1; i++)
        Left[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        Right[j] = arr[m + 1 + j];
    
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (Left[i] <= Right[j]) {
            arr[k] = Left[i];
            i++;
        } else {
            arr[k] = Right[j];
            j++;
        }
        k++;
    }
    
    
    while (i < n1) {
        arr[k] = Left[i];
        i++;
        k++;
    }
    
    
    while (j < n2) {
        arr[k] = Right[j];
        j++;
        k++;
    }
}
