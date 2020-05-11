/**
 * Pure C 2019.
 * File: ex1a.
 *
 * Authors: Tasneem Abu Odeh & Hanan Himouni.
 * id: 314902685, 318314663.
 * login: tasnimab + hananhi.
 *
 * A program that solves tower Hanoi by creating 5 children using fork, every
 * single child solves one Hanois tower problem, then creates a file and write the
 * solutions on it.
 *
 * It also gives as a stander output every Hanoi's tower and the time it tooks to
 * be solved.
 *
 *
 *
 * Input:
 * Output:A solutiuon for the Hanoi's tower problem and the time it took for every
 *          tower to be solved.
 *
 * Compile: gcc -Wall ex1a.c –o ex1a.
 * Run: ex1a.
 * RunTime: o(n^2), while n is the input size.
 *
 */

//include(s) section.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

//define(s) section.
#define arrSize 5
#define towerA 10
#define towerB 20
#define towerC 25
#define towerD 30
#define towerE 35

//prototype(s) section.
void checkChild(int status);
void help(FILE * fptr, int size);
void buildChild(FILE * fptr);
void hanoiTower(FILE * fptr, int size, char source, char target, char aux);

//main section.
int main(int argc, const char * argv[]) {
    
    FILE *fptr;
    fptr = fopen("write.txt", "w");
    
    if(fptr == NULL ){
        perror("Cannot open file");
        exit(1);
    }
    buildChild(fptr);
    fclose(fptr);
    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
//A function that builds the children whom going to solve the problem.
void buildChild(FILE * fptr){
    pid_t arr[arrSize];
    int i;
    for(i = 0; i < arrSize;  i++){
        
        arr[i]= fork();
        checkChild(arr[i]);
        
        if(arr[i] == 0){
            help(fptr, i);
            exit(0);
        }
        else wait(&arr[i]);
    }
    exit(0);
}

//------------------------------------------------------------------------------

//A function that checks if the child is valid.
void checkChild(int status){
    if(status < 0){
        perror("Error in fork\n");
        exit(1);
    }
    exit(0);
}

//------------------------------------------------------------------------------
/**A helping function to call for the Hanoi Tower function and finds the acceleration
of time it needed until it found the solution.*/
void help(FILE * fptr, int size){
    time_t start,
    theEnd;
    
    char source = '\0',
    target = '\0',
    aux = '\0';
    
    start = time(NULL);
    switch (size) {
        case 0:
            hanoiTower(fptr, towerA, source, target, aux);
            theEnd = time(NULL);
            printf("%d = %ld", towerA, (int)theEnd - start);
            break;
            
        case 1:
            hanoiTower(fptr, towerB, source, target, aux);
            theEnd = time(NULL);
            printf("%d = %ld", towerB, (int)theEnd - start);
            break;
            
        case 2:
            hanoiTower(fptr, towerC, source, target, aux);
            theEnd = time(NULL);
            printf("%d = %ld", towerC, (int)theEnd - start);
            break;
            
        case 3:
            hanoiTower(fptr, towerD, source, target, aux);
            theEnd = time(NULL);
            printf("%d = %ld", towerD, (int)theEnd - start);
            break;
            
        case 4:
            hanoiTower(fptr, towerE, source, target, aux);
            theEnd = time(NULL);
            printf("%d = %ld", towerE, (int)theEnd - start);
            break;
    }
    
}

//------------------------------------------------------------------------------
//A function that finds the solution for the Hanoi Tower.
void hanoiTower(FILE * fptr, int size, char source, char target, char aux) {
    if(size == 1)
        fprintf(fptr, "Move ring of size 1 from", source, "to", target, "\n");
    else {
        hanoiTower(fptr, size - 1, source, aux, target);
        fprintf(fptr, "Move ring of size ", size, " from ", source, " to ", target, "\n" );
        hanoiTower(fptr, size - 1, aux, target, source);
    }
}
