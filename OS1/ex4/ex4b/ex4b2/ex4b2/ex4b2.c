//include(s) section.
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

//Define(s) section.
#define MAX_TEXT 200

//Prototype(s) section.
int recMsg(char *FifoQueue);
void sendMsg(char [MAX_TEXT], char *FifoQueue);
void addFactor(char STR[MAX_TEXT], int num);
void primeFactors(char STR[MAX_TEXT], int num) ;
void signalHandler(int signum);
void checkPipe(FILE *fptr);

//main section.
int main(int argc, const char * argv[]) {
    int Val;
    char STR[MAX_TEXT],
    *FifoQueue="PrimeFifoQueue";
    
    memset(STR, 0, MAX_TEXT);
    
    signal(SIGINT, signalHandler);
    
    mkfifo(FifoQueue, S_IFIFO | 0644);
    
    
    for(;;){
        Val = recMsg(FifoQueue);
        primeFactors(STR, Val);
        sendMsg(STR, FifoQueue);
    }
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//A function that handles the SIGINT signal, it deletes the
//message queue.
void signalHandler(int signum) {
    system("rm PrimeFifoQueue");
    puts("Deleting messages queue!");
    exit(0);
    
}

//--------------------------------------------------------
// A function to print all prime factors of a given number.
void primeFactors(char STR[MAX_TEXT], int num) {
    int div;
    //printf("\nThe prime factors of %d are: \n\n",number);
    
    div = 2;
    
    while(num!=0){
        if(num%div!=0)
            div = div + 1;
        else {
            num = num / div;
            addFactor(STR,div);
            if(num == 1)
                break;
        }
    }
}

//--------------------------------------------------------
//A function to build queue to fill it with the number's
//Prime factors.
void addFactor(char STR[MAX_TEXT], int Val) {
    unsigned int length = strlen(STR);
    char temp[MAX_TEXT];
    
    STR[length]=' ';
    STR[length+1]='\0';
    
    sprintf(temp, "%d", Val);
    strcat(STR, temp);
}

//--------------------------------------------------------
//A function that initializes the sending messages queue.
void sendMsg(char STR[MAX_TEXT], char *FifoQueue){
    FILE *fdr = fopen(FifoQueue, "w");
    checkPipe(fdr);
    
    fprintf(fdr, "%s", STR);
    fflush(fdr);
    fclose(fdr);
}

//--------------------------------------------------------
//A function that initializes the receving messages queue.
// it returns the id of the queue.
int recMsg(char *FifoQueue){
    int Val;
    
    FILE *fdr = fopen(FifoQueue, "r");
    checkPipe(fdr);

    fscanf(fdr, "%d", &Val);
    fclose(fdr);
    
    return Val;
    
}

//--------------------------------------------------------
//A function to check if the Pipe Opened successfully.
void checkPipe(FILE *fptr){
    if(!fptr) {
        perror("Cannot open FifoQueue pipe:");
        exit(1);
    }
}


