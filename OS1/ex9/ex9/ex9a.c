/**
 * Pure C 2019
 * ex9a.c
 *
 *
 * Created on: Jan 16, 2019
 *        Created by Tasneem Abu Odeh - id: 314902685 - login: tasnimab.
 *             Jouana Nusralla - id:312315104 - login: jouananu.
 *
 *  This program generates random number between 2 and 1000 and it finds
 * which of them are prime. after that adds them to an array that is in
 * a shared memory, then after it reaches to fill the last one it makes
 * sure they dont clash by using semaphore. after that it will print
 * NOTE : it will take a minute or two , because of the sleep function.
 *
 *
 *  Copyright © 2018 Tasneem Abu Odeh. All rights reserved.
 *
 */
//———————————————————————————————————————————————————————————————————————————

//include(s) section.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

//Define(s) section.
#define VAL 998

//Struct(s) section.
typedef struct Son
{
    int id[2];
    int Values[10];
    int counter;
}Son;

//Prototype(s) section.
int createSHM(void);
int isPrime(int num);
void SonProcess1(int i,Son *pointer);
void SonProcess2(int i , Son *pointer);
void signalHandler1(int signum);
void signalHandler2(int signum);
void createMutex(void);
Son *createPointers(int shm_id);

//Global(s) section.
sem_t *mutex;



//main section.
int main(int argc, const char * argv[]) {
    //Gets randome numbers
    srand(17);
    
    //to catch signal handler.
    signal(SIGINT, signalHandler1);
    
    //Declares all the variables we need.
    int index,pid;
    Son *pointer;
    int shm_id;
    
    //Calls the function which is responsable to create MUTEX.
    createMutex();
    
    //Calls the function which is responsable to create SHM.
    shm_id = createSHM();
    
    //Calls the function which is responsable to create POINTERs.
    pointer = createPointers(shm_id);
    pointer->counter =0;
    
    for(index = 0; index < 2; index++) {
        pid = fork();
        if(pid == 0) {
            signal(SIGINT,signalHandler2);
            pointer->id[index] = getpid();
            if(index == 0) {
                SonProcess1(index, pointer);
            }
            if(index == 1) {
                SonProcess2(index, pointer);
            }
            sem_close(mutex);//closes the MUTEX.
            exit(0);
        }
    }
    wait(NULL);
    return(0);
}

//———————————————————————————————————————————————————————————————————————————
//A function that catches the siganl (SIGINT).
void signalHandler1(int signum){
    
    //Declares all the variables we need.
    key_t key;
    int shm_id;
    Son *pointer;
    
    //Creates the key.
    key = ftok(".",'a');
    
    //Checks if successeded.
    if((shm_id = shmget(key,0, 0600)) == -1) {
        perror("FAILD TO GET MEMORY ");
        //exit(1);
    }
    if((pointer = (Son*)shmat(shm_id,NULL,0))<0) {
        perror("FAILD TO ATTACH MEMORY ");
        //exit(1);
    }
    
    int index1, index2;
    for(index1 = 0; index1 < 2; index1++) {
        kill(pointer->id[index1],SIGINT);
    }
    //Prints out the values
    for(index2 = 0; index2 < 10; index2++) {
        printf("%d ",pointer->Values[index2]);
    }
    
    puts("");
    sem_close(mutex);
    sem_unlink("/MUTEX15");
    shmdt(pointer);
    shmctl(shm_id,IPC_RMID,NULL);
    exit(EXIT_SUCCESS);
}
//———————————————————————————————————————————————————————————————————————————
//A function that catches the siganl (SIGINT).
void signalHandler2(int signum) {
    exit(0);
}

//———————————————————————————————————————————————————————————————————————————
//A function that makes the pointer to the shm
Son *createPointers(int shm_id) {
    
    //Declares all the variables we need.
    Son *pointer;
    
    //Checks if the process successeded.
    if((pointer = (Son*)shmat(shm_id,NULL,0))<0) {
        perror("fFAILD TO ATTACH MEMORY ");
        exit(1);
    }
    return pointer;
}
//———————————————————————————————————————————————————————————————————————————
//this function makes the shared memory
int createSHM(void) {
    //Declares all the variables we need.
    key_t key;
    int shm_id;
    
    //Creates the key.
    key = ftok(".",'a');
    
    //Checks if the process successeded.
    if((shm_id = shmget(key,3,IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("FAILD TO GET MEMORY :");
        exit(1);
    }
    return shm_id;
}
//———————————————————————————————————————————————————————————————————————————
//a function that creates a MUTEX and chicks if it's right.
void createMutex() {
    mutex = sem_open("/MUTEX15",O_CREAT,0644,1);
//Checks if the mutex successeded
    if(mutex ==SEM_FAILED) {
        perror("PARENT SEM OPENNING FAILD :");
        exit(EXIT_FAILURE);
    }
}
//———————————————————————————————————————————————————————————————————————————
//This function is the first Son , generating random numbers
//then checks if it is prime number to send it to the shm
void SonProcess1(int i,Son *pointer) {
    while(1) {
        int num = rand()%998+2;
        if((isPrime(num)==1)) {
            sem_wait(mutex);
            sleep(5);
            pointer->Values[pointer->counter] = num;
            pointer->counter +=1;
            if(pointer->counter == 10) {
                kill(getppid(),SIGINT);
                exit(0);
            }
            sem_post(mutex);
            sleep(1);
        }
    }
}
//———————————————————————————————————————————————————————————————————————————
//This function is the second Son , generating random numbers
//then checks if it is prime number to send it to the shm
void SonProcess2(int i , Son *pointer) {
    while(1) {
        int num = rand()%VAL+2;
        
        if((isPrime(num) == 1)) {
            sem_wait(mutex);
            pointer->Values[pointer->counter] = num;
            pointer->counter +=1;
            if(pointer->counter == 10) {
                kill(getppid(),SIGINT);
                exit(0);
            }
            sem_post(mutex);
            sleep(1);
        }
    }
}
//———————————————————————————————————————————————————————————————————————————
//A function that finds if the number is prime or not, if it is prime
//it returns 1, else it returns 0.
int isPrime(int num) {
    int i;
    if (num <= 1) return 0;
    if (num % 2 == 0 && num > 2) return 0;
    for(i = 3; i < num / 2; i+= 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

