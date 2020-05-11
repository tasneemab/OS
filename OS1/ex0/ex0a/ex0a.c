/**
 * Pure C 2019.
 * File: ex0a.
 *
 * Authors: Tasneem Abu Odeh & Hanan Himouni.
 * id: 314902685, 318314663.
 * login: tasnimab + hananhi.
 *
 * A program that creates a dynamic array (2 dimensional), and finds which row
 * contains the greatest values. It takes a row and checks its columns if it's
 * greater than the other rows.
 *
 * if there's a row that maintains the wanted the program returns it's number,
 * and if there's not a one it returns -1.
 *
 * Input: File that contains numbers.
 * Output: The number of the greatest row.
 *
 * Compile: gcc -Wall ex0a.c –o ex0a.
 * Run: ex0a.
 * RunTime: o(n^2), while n is the input size.
 *
 */

//includes section
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//structs section
typedef struct {
    int numOfLines;
    int ** theData;
    int * linesLen;
} Data;

//defines section

//protottypes
void readFromFileToArray(FILE * fiptr);
void freeMemory(int usedRows, int ** arr);
void biggestRow(Data data);
int isBigger(int num,Data data,int col);

//main section
int main(int argc, const char * argv[]) {
    FILE *fptr;
    fptr = fopen(argv[1], "r");

    if(argc != 2 || fptr == NULL ){
        perror("Cannot open file");
        exit(1);
    }

    readFromFileToArray(fptr);

    fclose(fptr);

    return EXIT_SUCCESS;
}


//A function to pass from file to array.
void readFromFileToArray(FILE * fiptr){
    int i;
    int num;
    int realRows = 0;
    int usedRows = 0;

    Data ptr;
    ptr.linesLen = NULL;

    while(feof(fiptr)){
        fscanf(fiptr, "%d", &num);

        if(ptr.linesLen == NULL){
            ptr.linesLen=(int*)malloc(sizeof(int));
        }
        else {
            ptr.linesLen= realloc(ptr.linesLen,(usedRows+1)*sizeof(int));
        }
        ptr.linesLen[usedRows] = num;

        if(realRows == usedRows){
            ptr.theData = (int **)realloc(ptr.theData, ((realRows == 0) ? 1 :
                                                        realRows * 2) * sizeof(int *));

            if(ptr.theData == NULL){
                fputs("Cannot allocate memory\n", stderr);
                exit(EXIT_FAILURE);
            }
            realRows = (realRows == 0) ? 1 : realRows * 2;

        }
        ptr.theData[usedRows] = (int *)malloc((num) * sizeof(int));

        if(ptr.theData[usedRows] == NULL){
            fputs("Cannot allocate memory\n", stderr);
            exit(EXIT_FAILURE);
        }

        for(i = 0; i < num; i++)
            fscanf(fiptr ,"%d",&(ptr.theData[usedRows][i]));

        usedRows++ ;

        biggestRow(ptr);
        freeMemory(usedRows, ptr.theData);
    }

}

//A function to free memory.
void freeMemory(int usedRows, int ** arr){
    int row;
    for(row = 0; row < usedRows; row++)
        free(arr[row]);
    free(arr);
}

//A function to find the wanted row.
void biggestRow(Data data){
    int row;
    int col;
    for(row=0;row<data.numOfLines;row++){
        for(col=0;col<data.linesLen[row];col++)
        {
            if(isBigger(data.theData[row][col],data,col) == 0){
                break;
            }
            if(col == data.linesLen[row]-1){
                printf("%d",row);
                return;
            }
        }
    }
    printf("%d",-1);
}

//A function that checks if the value is bigger that the others in the same calumn.
int isBigger(int num,Data data,int col){
    int row=0;
    for(row = 0; row<data.numOfLines; row++){
        if(data.linesLen[row]>=col){
            if(num<data.theData[row][col]){
                return 0;
            }

        }
    }
    return 1;
}