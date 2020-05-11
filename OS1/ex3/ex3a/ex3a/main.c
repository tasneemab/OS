/**
 * Pure C 2019.
 * File: ex3a.
 *
 * Authors: Tasneem Abu Odeh & Anwar Hamdan.
 * id: 314902685, 319109609
 ';.
 * login: tasnimab + anwarha.
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
 * Input: Nothing.
 * output: each tower = time of the process.
 *
 * Compile: gcc -Wall ex3a.c –o ex3a.
 * Run: ex3a.
 * RunTime: O(n^2), while n is the input size.
 *
 */

//include section
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

//define section
#define SIZE 5
#define towerA 5
#define towerB 10
#define towerC 15
#define towerD 20
#define towerE 25

//global(s) section
pid_t  status;
int pipeDescs[2];

//prototype(s) section.
void checkFork(void);
void sonProcess(void);
void fatherProcess(void);
void process(pid_t status);
void hanoiTower(FILE * fptr, int size, char source, char target, char aux);

//main section
int main() {
    int i;
    if(pipe(pipeDescs) == -1){
        perror("Cannot Pipe:");
        exit(1);
    }
    
    for(i = 0; i< SIZE; i++){
        status = fork();
        checkFork();
        process(status);
    }
    
    exit(EXIT_SUCCESS);
}

//--------------------------------------------------
//A function that checks if the child is valid.
void checkFork(){
    if(status < 0){
        perror("Cannot fork:");
        exit(1);
    }
    else if(status > 0)
        fatherProcess();
    else
        sonProcess();
    
}

//--------------------------------------------------
//A function that do child's process.
void process(pid_t status){
    waitpid(status, NULL, 0);
}
//--------------------------------------------------
//A function that do the son process.
void fatherProcess(){
    
    int k;
    int infoTime;
    int numOfRings, maxTime = 0, minTime = 100000;
    double sum;
    int ARR[SIZE] = {towerA, towerB, towerC, towerD, towerE};
    
    close(pipeDescs[1]);
    dup2(pipeDescs[0],STDIN_FILENO);
    sum = 0;
    for(k = 0; k < SIZE; k++){
        scanf("%d", &numOfRings);
        if(numOfRings != ARR[k]){
            perror("Cannot Continue:");
            exit(EXIT_FAILURE);
        }
        scanf("%d", &infoTime);
        
        if(infoTime > maxTime){
            maxTime = infoTime;
        }
        if(infoTime < minTime){
            minTime = infoTime;
        }
        sum += infoTime;
        
        printf("num of rings = %d, minTime = %d, maxTime = %d, avgTime = %lf. \n",ARR[k],
               minTime, maxTime, (sum/5));
        
    }
}

//--------------------------------------------------
//A function that do the father process.
void sonProcess(){
    
    time_t seconds = 0, tempT = 0;
    FILE *fptr = fopen("/dev/null", "a");
    
    int towers[SIZE] = {towerA, towerB, towerC, towerD, towerE};
    int i;
    close(pipeDescs[0]);
    dup2(pipeDescs[1],STDOUT_FILENO);
    
    for(i = 0; i< SIZE; i++){
        srand(time(NULL));
        printf("%d " , towers[i]);
        seconds = time(NULL);
        hanoiTower(fptr, towers[i], 'a', 'b', 'c')    ;
        tempT = time(NULL);
        seconds = tempT - seconds;
        printf("%d ", (int)seconds);
    }
}

//--------------------------------------------------
//A function that finds the solution for the Hanoi Tower.
void hanoiTower(FILE * fptr, int size, char source, char target, char aux) {
    if(size == 1){
        fprintf(fptr," Move disk 1 from rod %c to rod %c\n", source, target);
        return;
    }
    hanoiTower(fptr,size-1, source, aux, target);
    fprintf(fptr,"Move disk %d from rod %c to rod %c\n", size, source, target);
    hanoiTower(fptr,size-1, aux, target, source);
}
