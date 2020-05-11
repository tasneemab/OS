
//include(s) section.
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

//Define(s) section.
#define MAX_TEXT 200

//Prototype(s) section.
int GCD(int Val1, int Val2);
void signalHandler(int signum);
void recMsg(char *FifoQueue, int *Val1, int *Val2);
void sendMsg(char *FifoQueue, int Val);
void checkPipe(FILE *fptr);

//main section.
int main(int argc, const char * argv[]) {
    int Val1, Val2;
    
    char *FifoQueue="GCD_FifoQueue";
    
    signal(SIGINT, signalHandler);
    mkfifo(FifoQueue, S_IFIFO | 0644);
    
    for(;;) {
        recMsg(FifoQueue, &Val1, &Val2);
        sendMsg(FifoQueue, GCD(Val1,Val2));
    }
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//A function that handles the SIGINT signal, it deletes the message queue.
void signalHandler(int signum){
    system("rm GCD_FifoQueue");
    puts("Deleting messages queue!");
    exit(0);
}
//--------------------------------------------------------
//A function to find the GCD for two values.
int GCD(int Val1, int Val2){
    int remainder = Val1 % Val2;
    
    if (remainder == 0) {
        return Val2;
    }
    
    return GCD(Val2, remainder);
}

//--------------------------------------------------------
//A function that initializes the sending messages queue.
void sendMsg(char *FifoQueue, int Val){
    
    FILE *fdr = fopen(FifoQueue, "w");
    checkPipe(fdr);
    fprintf(fdr, "%d", Val);
    fflush(fdr);
    fclose(fdr);
}

//--------------------------------------------------------
//A function that initializes the receving messages queue.
void recMsg(char *FifoQueue, int *Val1, int *Val2){
    
    FILE *fdr = fopen(FifoQueue, "r");
    checkPipe(fdr);
    
    fscanf(fdr, "%d %d", &*Val1, &*Val2);
    fclose(fdr);
}

//--------------------------------------------------------
//A function to check if the Pipe Opend successfully.
void checkPipe(FILE *fptr){
    if(!fptr) {
        perror("Cannot open FifoQueue pipe:");
        exit(1);
    }
}
