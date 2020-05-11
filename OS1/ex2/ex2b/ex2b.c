/* ex2b: Signals between processes
 * =======================================================================
 * Written by: Tasneem Abu Odeh, ID: 314902685, Login: tasnimab
 * =======================================================================
 *
 * This program creates two processes, they "stay alive" according to
 * a generated random number, they send signals to each other..
 * when one signal ends, the other waits 10 seconds for it, and then
 * if it didn't get any signal from it, it ends as well.
 *
 * Input: Nothing.
 * Output: The progress of the signals between the processes to stdin.
 * =======================================================================
 */

//include(s) section.
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>

//define(s) section.
#define MAX_RANDOM 8
#define SLEEP_TIME 10

//static(s) section.
static volatile sig_atomic_t sig = 0;

//prototype(s) section.
/**
 * This function handles SIGUSR1 signal,
 * and prints to stdin that it got signal from other process,
 * in other words, that the current process has a partner.
 */
void signalHandler(int signum);

/**
 * This function waits for signal for 10 seconds, if it gets one,
 * that's okay, if not, it kills the current process.
 */
void receiveSignal();

/**
 * This function contains an infinite loop, each time it
 * creates a random number, and depends on it, whether it ends
 * the process or sends a signal to its partner and waits for
 * one as well.
 */
void mainLoop(pid_t otherPid);

//main section.
int main(int argc, char* argv[])
{
    signal(SIGUSR1, &signalHandler);
    pid_t otherPid;
    pid_t d = fork();
    if(d < 0){
        perror("Cannot fork!");
        exit(EXIT_FAILURE);
    }
    if(d == 0){
        otherPid = getppid();
        srand(time(NULL));
    }
    else{
        otherPid = d;
        srand(time(NULL) + 1);
    }
    mainLoop(otherPid);
    return EXIT_SUCCESS;
    
}

//-------------------------------------------------------------------
void signalHandler(int signum){
    printf("process %d has a partner\n", getpid());
    sig = 1;
}

//-------------------------------------------------------------------
void receiveSignal()
{
    signal(SIGUSR1, &signalHandler);
    sleep(SLEEP_TIME);
    if (sig != 0){
        printf("process %d has a partner\n", getpid());
        sig = 0;
    }
    else
    {
        printf("process %d was left alone, and quits\n", getpid());
        exit(EXIT_SUCCESS);
    }
}

//-------------------------------------------------------------------
void mainLoop(pid_t otherPid){
    int random;
    while(1){
        random = rand() % MAX_RANDOM;
        if(0 < random < 4){
            printf("process %d ends\n", getpid());
            exit(EXIT_SUCCESS);
        }
        else{
            kill(otherPid,SIGUSR1);
            receiveSignal();
        }
    }
}
