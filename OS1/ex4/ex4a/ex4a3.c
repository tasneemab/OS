//include(s) section.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

//Define(s) section.
#define MAX_TEXT 200

//Global(s) section
int msgid;

//Struct(s) section.
//This struct stores the data of the message.
struct Data {
    int pid;
    int num;
};

//This struct represents the message.
struct msgbuff{
    long mtype;
    char text[MAX_TEXT];
    struct Data data;
};

//prototype(s) section.
void sendMsg(int queue_id,     struct msgbuff msg);
void GCDAppServer(int arr[]);
void PrimeFactorsAppServer(int arr[]);
void adding(char text[MAX_TEXT], int num);
void errorMsg(void);
void check(int arr[]);
int buildMessagesQueue(char c);
int *extractArgs(char **text, int size);
struct msgbuff server(int Msgid,  struct msgbuff msg);

//main section.
int main(int argc, char **argv) {
    int *arr;
    
    if(argc!=3 || argc!=4)
        errorMsg();
    
    arr=extractArgs(argv, argc-1);
    check(&arr[0]);
    
    free(arr);
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//A function to print out an error message when it's needed.
void errorMsg(void){
    
    perror("Openning failed:");
    exit(EXIT_FAILURE);
    
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
//A function that initializes the messages queue.
// it returns the id of the queue.
int buildMessagesQueue(char c){
    key_t key;
    if((key = ftok(".",'r')) == -1){
        fprintf(stderr, "ftok failed!\n");
        exit(EXIT_FAILURE);
    }
    if((msgid = msgget(key,IPC_CREAT | IPC_EXCL | 0600)) == -1){
        fprintf(stderr, "msgget failed!\n");
        exit(EXIT_FAILURE);
    }
    return msgid;
}

//--------------------------------------------------------
//A function to pass the client to the GCD app server.
void GCDAppServer(int arr[]) {
    struct msgbuff msg;
    buildMessagesQueue('g');
    
    
    adding(msg.text, arr[1]);
    adding(msg.text, arr[2]);
    
    msg.mtype = 1;
    msg = server(msgid, msg);
    printf("%s\n", msg.text);
    
}

//--------------------------------------------------------
//A function to pass the client to the PrimeFactors app server.
void PrimeFactorsAppServer(int arr[]) {
    struct msgbuff msg;
    buildMessagesQueue('d');
    
    adding(msg.text, arr[1]);
    
    msg.mtype = 1;
    msg = server(msgid, msg);
    printf("%s\n", msg.text);
    
}

//--------------------------------------------------------
//This function handles the app server, it receives
// messages from clients,sends messages to reg server to
// validate clients, calls the neededfunctions to perform what
// the client needs.
struct msgbuff server (int Msgid,  struct msgbuff msg){
    
    struct msgbuff mmsg, buff;
    int i;
    
    if(msgsnd(Msgid, &msg, sizeof(msg.text), 0) == -1){
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }
    
    if(msgrcv(Msgid, &buff, sizeof(mmsg.text), 1, 0)==-1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
        
    }
    mmsg.mtype=buff.mtype;
    
    for(i=0; i<MAX_TEXT; i++)
        if(buff.text[i]=='\0')
            break;
        else
            mmsg.text[i]=buff.text[i];
            return mmsg;
}

//--------------------------------------------------------
//A function to add values dependding on what the client
//choose.
void adding(char text[MAX_TEXT], int num) {
    char temp[MAX_TEXT];
    
    int length = strlen(text);
    
    text[length]=' ';
    text[length+1]='\0';
    sprintf(temp, "%d", num);
    strcat(text, temp);
}
//--------------------------------------------------------
//A function to allocate memory.
int *extractArgs(char **text, int size){
    int i,
    *arr =(int*) malloc(sizeof(int)*size);
    
    for(i=1; i<=size; i++)
        arr[i-1]=atoi(text[i]);
    
    return arr;
}
