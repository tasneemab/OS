/**
 * Pure C 2019
 * ex9b.c
 *
 *
 * Created on: Jan 16, 2019
 *        Created by Tasneem Abu Odeh - id: 314902685 - login: tasnimab.
 *             Jouana Nusralla - id:312315104 - login: jouananu.
 *
 *  This programs father forks two sons and in each son makes an arrays
 * size that is 10 , and then generates random numbers into each array from
 * 1 to 100 and then it generates in the father random numbers aswell from 1 to
 * 100 and sends it to the sons, first son to find all the generated
 * numbers is the winner. and the mutex blocks the posssiblity of them
 * finishing at same time. and "Race" problemss
 *
 *
 *  Copyright © 2018 Tasneem Abu Odeh. All rights reserved.
 *
 */
//———————————————————————————————————————————————————————————————————————————

//include(s) section.
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

//Define(s) section.
#define VAL 100

//Struct(s) section.
typedef struct ProcessID{
    int pid;
    int num;
}ProcessID;

//Prototype(s) section.
sem_t *mutex;
void sonProcess(int index);
void fatherProcess(void);
void createKey(key_t *key1);
void signalHandler1(int signum);
void signalHandler2(int signum);
void SearchAndFind(int numbersArray[],int num, int *counter,int booleansArray[]);

//Global(s) section.
ProcessID *pointer;
int shm_id;
int shm_id1;
int shm_id2;

//main section.
int main(int argc, const char * argv[]) {
   
    //Declares all the variables we need.
    int index, pid;
    key_t key1;
    
    //Creates the key.
    createKey(&key1);
    
    //Creates the MUTEX.
    mutex = sem_open("/MUTEX16",O_CREAT,0644,1);
    
    //sem_init(mutex,0,1);
    //Checks if the process successeded.
    if(mutex == SEM_FAILED) {
        perror("PARENT SEM OPENNING FAILD :");
        return EXIT_FAILURE;
    }
    
    //Creates the FORK processes.
    for(index = 0; index < 2; index++) {
        
        pid = fork();
        if(pid == 0) {
            sonProcess(index);
        }
    }
    fatherProcess();
//Gives time for the father.
    sleep(1);
    signal(SIGUSR1,signalHandler1);
    srand(time(NULL)+index);
    
    
    pause();
    exit(0);
}


//———————————————————————————————————————————————————————————————————————————
//A function that catches the siganl (SIGINT).
void signalHandler1(int signum) {
    
    //kills other son and free message queue
    kill(pointer[0].pid,SIGUSR2);
    kill(pointer[1].pid,SIGUSR2);
    
    if (shmctl(shm_id,IPC_RMID, NULL) == -1) {
        perror( "shmctl failed" ) ;
        exit( EXIT_FAILURE ) ;
    }
    sem_close(mutex);
    sem_unlink("/my_mutex20");
    exit(EXIT_SUCCESS);
    
}
//———————————————————————————————————————————————————————————————————————————
//A function that catches the siganl (SIGINT).
void signalHandler2(int signum) {
    exit(0);
}

//———————————————————————————————————————————————————————————————————————————
//runs the father , generates numbers and sends to sons
void fatherProcess() {
    //Declares all the variables we need.
    int GenNum;
    
    sleep(1);
    signal(SIGUSR1,signalHandler1);
    srand(time(NULL)+2);
    while(1) {
        
        if((pointer[3].num == 0) && (pointer[4].num == 0)) {
            GenNum = rand()%VAL + 1;
            pointer[2].num = GenNum;
            pointer[3].num = GenNum;
        }
    }
}
//———————————————————————————————————————————————————————————————————————————
//son (1/2) that make arrays with generated nums and then it recivies from
//the father to do "bingo"
void sonProcess(int index) {
    
    //to catch signal handler.
    signal(SIGUSR2,signalHandler2);
    pointer[index].pid = getpid();
    
    srand(time(NULL) + index);
    
    //Declares all the variables we need.
    int numArr[10],booleanArr[10],index2;
    
    for(index2 = 0; index2 < 10;index2++)
        numArr[index2] = rand()%VAL+1;
    for(index2 = 0; index2 < 10; index2++)
        booleanArr[index2] = 0;
    
    //Declares all the variables we need.
    int counter =0;
    pointer[index+2].num = 0;
    
    while(1) {
        if(pointer[index+2].num != 0) {
            SearchAndFind(numArr,pointer[index+2].num,&counter,booleanArr);
            pointer[index+2].num = 0;
        }
        if(counter == 10) {
            sem_wait(mutex);
            
            //Declares all the variables we need.
            int index3;
            printf("The Winner son is %d :",index+1);
            for(index3 = 0; index3 < 10; index3++) {
                printf("%d ",numArr[index3]);
            }
            puts("");
            kill(getppid(),SIGUSR1);
            sem_post(mutex);
            exit(0);
        }
    }
}

//———————————————————————————————————————————————————————————————————————————
//This function finds if a number which is given from father is in son array
//and then fils its place in boolan array with 1 n promotes counter
void SearchAndFind(int numbersArray[],int num, int *counter,int booleansArray[]) {
    int index;
    
    for(index = 0; index < 10; index++) {
        
        //if its equal to num and booleana place is 0
        if((numbersArray[index]==num)&&(booleansArray[index] ==0)) {
            booleansArray[index]=1;
            *counter+=1;
            break;
        }
    }
}

//———————————————————————————————————————————————————————————————————————————
//This function creates the needed key and sets the shared memory.
void createKey(key_t *key1) {
    
    *key1 = ftok(".",'f');
    
    //Checks if the process successeded.
    if((shm_id = shmget(*key1,1000,IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("FAILD TO GET MEMORY");
        exit(1);
    }
    
    if((pointer = (ProcessID*)shmat(shm_id,NULL,0))<0) {
        perror("FAILD TO ATTACH MEMORY");
        exit(1);
    }
}
