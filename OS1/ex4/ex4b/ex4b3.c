
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
void check(int arr[]);
void errorMsg(void);
void GCDAppServer(int *arr);
void PrimeFactorsAppServer(int *arr);
void adding(char STR[MAX_TEXT], int num);
void sendMsg(char *FifoQueue, char STR[MAX_TEXT]);
void checkPipe(FILE *fptr);
char *receiveMsg(char *FifoQueue);
int *extractArgs(char **text, int size);


//main section.
int main(int argc, char **argv) {
    
    if(argc!=3 || argc!=4)
        errorMsg();
    
    int *arr;
    arr=extractArgs(argv, argc-1);
    check(&arr[0]);
    
    free(arr);
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//A function to print out an error message when it's needed.
void errorMsg(){
    perror("Error in openning:");
    exit(1);
}

//--------------------------------------------------------
//A function to check which server the client wants to use.
void check(int arr[]){
    if(arr[0]==0)
        GCDAppServer(arr);
    
    else if(arr[0]==1)
        PrimeFactorsAppServer(arr);
    
}

//--------------------------------------------------------
//A function to pass the client to the GCD app server.
void GCDAppServer(int *arr) {
    char text[MAX_TEXT],
    *FifoQueue = "GCD_FifoQueue";
    
    mkfifo(FifoQueue, S_IFIFO | 0644);
    
    adding(text, arr[1]);
    adding(text, arr[2]);
    
    sendMsg(FifoQueue, text);
    printf("%s\n", receiveMsg(FifoQueue));
    
}

//--------------------------------------------------------
//A function to pass the client to the PrimeFactors app server.
void PrimeFactorsAppServer(int *arr) {
    char text[MAX_TEXT],
    *FifoQueue = "PrimeFifoQueue";
    mkfifo(FifoQueue, S_IFIFO | 0644);
    
    adding(text, arr[1]);
    
    sendMsg(FifoQueue, text);
    printf("%s\n", receiveMsg(FifoQueue));
    
}

//--------------------------------------------------------
//A function to add values dependding on what the client
//choose.
void adding(char STR[MAX_TEXT], int num) {
    char temp[MAX_TEXT];
    
    unsigned int length = strlen(STR);
    
    STR[length]=' ';
    STR[length+1]='\0';
    sprintf(temp, "%d", num);
    strcat(STR, temp);
}

//--------------------------------------------------------
//A function that initializes the sending messages queue.
void sendMsg(char *FifoQueue, char STR[MAX_TEXT]) {
    FILE *fdr = fopen(FifoQueue, "w");
    if(fdr == NULL)
        checkPipe(fdr);
    
    fprintf(fdr, "%s", STR);
    fflush(fdr);
    fclose(fdr);
}

//--------------------------------------------------------
//A function that initializes the receving messages queue.
char *receiveMsg(char *FifoQueue) {
    
    char *text = (char*) malloc(sizeof(char)*MAX_TEXT);
    
    FILE *fdr = fopen(FifoQueue, "r");
    if(fdr == NULL)
        checkPipe(fdr);
    
    fgets(text, MAX_TEXT, fdr);
    fclose(fdr);
    
    return text;
}

//--------------------------------------------------------
//A function to check if the Pipe Opened successfully.
void checkPipe(FILE *fptr){
    if(!fptr) {
        perror("Cannot open FifoQueue pipe:");
        exit(1);
    }
}

//--------------------------------------------------------
//A function to allocate memory.
int *extractArgs(char **text, int size) {
    int i,
    *arr =(int*) malloc(sizeof(int)*size);
    
    for(i=1; i<=size; i++)
        arr[i-1]=atoi(text[i]);
    
    return arr;
}
