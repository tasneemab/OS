/*
 * Pure C 2019
 * ex8a.c
 *
 *
 * Created on: Jan 1, 2019
 *        Created by Tasneem Abu Odeh - id: 314902685 - login: tasnimab.
 *             Jouana Nusralla - id:312315104 - login: jouananu.
 *
 *  This is a program which builds a dynamic array that has 10 cells,
 * fill it fith random numbers between 1 and 1000  using srand(17).
 * The main process in the program produces five sub-threads, every
 * sub-thread sorts a fifth of the array, then the main process sorts the
 * the five parts of the array at once.
 *
 *
 *  Copyright © 2018 Tasneem Abu Odeh. All rights reserved.
 *
 */
//———————————————————————————————————————————————————————————————————————————

//include(s) section.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

//Define(s) section.
#define SIZE 10
#define MAXLEN 50
#define THREADSNUM 5
#define RANGE 1000

//Global(s) section.
int ** ARRAY;
pthread_key_t key;
pthread_once_t threadsInit = PTHREAD_ONCE_INIT;

//Prototype(s) section.
void * my_func(void * args);
void merge(int ** array);
void sort(void);
void getKey(void);
void checkAllocation(int * allocs);
void checkThreadCreatin(int status);
int cmpfunc (const void * a, const void * b);
             
//main section.
int main(int argc, const char * argv[]) {
    
    //Indicates all the variables we need.
    int status;
    int index;
    int *ret = NULL;
  
    
    pthread_t pthreadData[THREADSNUM];//Array thread
    srand(17);//Random number
    
    //Threads array allocation in memory.
    ARRAY = (int**)malloc(THREADSNUM * sizeof(int*));
    checkAllocation(*ARRAY);

    //Create threads
    for(index = 0; index < THREADSNUM; index++) {
        status = pthread_create(&(pthreadData[index]),NULL, my_func, NULL);
        checkThreadCreatin(status);  //Checks the threads.
    }
    
    //Wait the threads
    for(index = 0; index < THREADSNUM; index++) {
        status = pthread_join(pthreadData[index],(void **) &ret);
        ARRAY[index] = ret;
        
        /**for(index2 = 0; index2 < SIZE; index2++)
            printf("%d ", *(ret+index2));
        puts(" ");**/
    }
    merge(ARRAY);
    free(ARRAY);//Frees memory
    free(ret);//Frees memory

    return EXIT_SUCCESS;
}
//———————————————————————————————————————————————————————————————————————————
//This function checks the memory allocation if  successeded.
void checkAllocation(int * alloc){
    if(alloc == NULL){
        perror("Cannot allocate memory ");
        exit(EXIT_FAILURE);
    }
}

//———————————————————————————————————————————————————————————————————————————
//This function checks the threads creation if successeded.
void checkThreadCreatin(int status){
    if(status != 0) {
        perror("Pthread craetion failed in main");
        exit(EXIT_FAILURE);
    }
}

//———————————————————————————————————————————————————————————————————————————
//The main function in the program that runs the whole process.
void * my_func(void * args){
    int rc;
    int *ARR;
    size_t index;

    ARR = (int*) malloc (SIZE *sizeof(int));//Array allocation in memory.
    checkAllocation(ARR); //Checks if the allocation was successful.
    
    pthread_once(&threadsInit, getKey);//Gets the key
    rc = pthread_setspecific(key,ARRAY);

   
    
    for(index = 0; index < SIZE; index++)//Fills the array with random numbers.
        ARR[index] = rand() % RANGE;
  
    
    if(rc) {
        fputs("Pthread setspecific failed", stderr);//Checks the threads setting.
        exit(EXIT_FAILURE);
    }
    
    sleep(rand() % 10) ;
    sort();
    
    free(ARR);//Frees memory.
    pthread_exit(ARR);//Threads exit.
}

//———————————————————————————————————————————————————————————————————————————
//This function gets the needed key.
void getKey(void){
    int rc;
    rc = pthread_key_create(&key,NULL);

    if(rc) {
        fputs("Pthread key creation failed !", stderr);
        exit(EXIT_FAILURE);
    }
}

//———————————————————————————————————————————————————————————————————————————
//This function sorts the array.
void sort(void){
    int *ARR;
    ARR = (int*) pthread_getspecific(key);
    qsort(ARR, SIZE, sizeof(int), cmpfunc);
    free(ARR);//Frees memory
}

//———————————————————————————————————————————————————————————————————————————
//This function helps the quiq sort to compare between the values.
int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//———————————————————————————————————————————————————————————————————————————
//This function merges all the subs arraies after they been sorted.
void merge(int ** array){
    //Indicates all the variables we need.
    int * ARR;
    int index1, index2;
    int temp;
    int len = 0;
    
    //Array allocation in memory.
    ARR = (int*)malloc(MAXLEN *sizeof(int));
    checkAllocation(ARR);//Checks the allocation if successeded.
    
    for(index1 = 0; index1 < THREADSNUM; index1++){
        for(index2 = 0; index2 < SIZE; index2++){
            ARR[index1] = array[index1][index2];
            len++;
        }
    }
    
    //Merges the sub-arries.
    for(index2 = 0; index2 < MAXLEN -1; index2++){
        for(index1 = 0; index1 < MAXLEN - index2 -1; index1++){
            if(ARR[index1] > ARR[index1+1]){
                temp = ARR[index1];
                ARR[index1] = ARR[index1+1];
                ARR[index1+1] = temp;
            }

        }
    }
    //Prints out the array's values.
    for(index1 = 0; index1 < MAXLEN; index1++)
        printf("%d ",ARR[index1]);
    putchar('\n'); //Prints endl.
    free(ARR);//Frees memory
}
