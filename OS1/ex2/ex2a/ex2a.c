/* ex2a: Signals between processes
 * =======================================================================
 * Written by: Tasneem Abu Odeh, ID: 31902685, Login: tasnimab
 * =======================================================================
 *
 * This program creates two processes, and they send signals to each
 * other randomly, and the one who gets 5 signals of the same type,
 * sends a message that he's surrendering, and the other wins.
 *
 * Input: Nothing
 * 
 * Output: Messages of the progress of the signals to stdin.
 * =======================================================================
 */

//include(s) section
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>

//define(s) section
#define NUM_OF_ITERS 10
#define WIN_LIMIT 2
#define SECONDS 4

//global(s) section
int sigCount1 = 0,
sigCount2 = 0;

/*
 * This function handles SIGUSR1/SIGUSR2 signals,
 * and prints to stdin what signal the process got.
 */
void signalHandler1(int signum);

/*
 * This function handles SIGTERM signals,
 * and prints to stdin what signal the process got.
 */
void signalHandler2(int signum);

/*
 * This is the main function in the program, it goes in a loop
 * each time with a random number, and sends signals between
 * the two process according to the random number.
 * Until one of them wins.
 */
void doProcess(pid_t p);

int main(int argc, char* argv[])
{
    signal(SIGUSR1, &signalHandler1);
    sigCount1++;
    signal(SIGTERM, &signalHandler2);
    sigCount2++;
    
    pid_t d = fork();
    if(d < 0){
        perror("Cannot fork!");
        exit(EXIT_FAILURE);
    }
    doProcess(d);
    return EXIT_SUCCESS;
    
}
//-------------------------------------------------------------------
void signalHandler1(int signum){
    if(signum == SIGUSR1){
        printf("process %d got signal SIGUSR1\n",getpid());
        signal(SIGUSR1, &signalHandler1);
    }
}
//-------------------------------------------------------------------
void signalHandler2(int signum){
    printf("process %d win\n", getpid());
    exit(EXIT_SUCCESS);
}
//-------------------------------------------------------------------
void doProcess(pid_t p){
    pid_t otherPid;
    
    if(p == 0){
        otherPid = getppid();
    }
    else{
        otherPid = p;
    }
    
    int counter1 = 0,
    counter2 = 0,
    i, random;
    
    if(p == 0){
        srand(time(NULL));
    }
    else
        srand(time(NULL) + 1);
    
    for (i = 0; i < NUM_OF_ITERS; i++) {
        random = rand() % SECONDS;
        for(random = rand() % SECONDS; random > 0; random--)
            sleep(1);
        if((rand() % WIN_LIMIT) == 0){
            kill(otherPid, SIGUSR1);
            counter1++;
        }
        else
            counter2++;
       
        if((sigCount1 >= WIN_LIMIT && counter1 >= 2*WIN_LIMIT)||
           (sigCount2 >= WIN_LIMIT && counter2 >= 2*WIN_LIMIT) ){
            printf("process %d surrender\n",getpid());
            kill(otherPid, SIGTERM);
            if(p != 0)
                wait(NULL);
            exit(EXIT_SUCCESS);
        }
    }
}
