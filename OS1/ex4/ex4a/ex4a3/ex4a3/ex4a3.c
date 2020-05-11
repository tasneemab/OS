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
void GCDAppServer(char ** argv);
void PrimeFactorsAppServer(char ** argv);
void adding(char text[MAX_TEXT], int num);
void errorMsg(void);
void check(int arr[]);
void reset(void);
int buildMessagesQueue(char c);
int *extractArgs(char **text, int size);
struct msgbuff server(int Msgid,  struct msgbuff msg);

//main section.
int main(int argc, char **argv) {
    
    if(argc < 3 || argc > 4)
        errorMsg();
    reset();
   
if(atoi(argv[1]) == 0)
    GCDAppServer(&argv[1]);
    else
        PrimeFactorsAppServer(&argv[1]);
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//A function to print out an error message when it's needed.
void errorMsg(void){
    
    perror("Openning failed:");
    exit(EXIT_FAILURE);
    
}

//--------------------------------------------------------
void reset(){
    struct msgbuff myMsg;
    int i;
    
    for(i = 0; i < MAX_TEXT; i++)
        myMsg.text[i] = 0;
}

//--------------------------------------------------------
//A function to check which server the client wants to use.
/**void check(int arr[]){
    if(arr[0] == 0)
        GCDAppServer(arr);
    
    else if(arr[0] == 1)
        PrimeFactorsAppServer(arr);
    
}**/

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
void GCDAppServer(char ** argv) {
    struct msgbuff msg;
    buildMessagesQueue('g');
    
    msg.text[0] = atoi(argv[2]);
    msg.text[1] = atoi(argv[3]);
 
    
    printf("the GCD is %d\n", msg.text[2]);
    printf("\n");
    
    msg.mtype = 1;
    msg = server(msgid, msg);
    
}

//--------------------------------------------------------
//A function to pass the client to the PrimeFactors app server.
void PrimeFactorsAppServer(char ** argv) {
    struct msgbuff msg;
    buildMessagesQueue('d');
    
    msg.text[0] = atoi(argv[2]);
    
    int i;
    printf("the primary factors are:\n");
    for(i = 0; i < MAX_TEXT; i++){
        if(msg.text[i] == 0)
            break;
        printf("%d ", msg.text[i]);
    }
        printf("\n");
        
    
        msg.mtype = 1;
        msg = server(msgid, msg);
    
    }

//--------------------------------------------------------
    //
    struct msgbuff server (int Msgid,  struct msgbuff msg){
        
        struct msgbuff mmsg, buff;
        int i;
        
        if(msgsnd(Msgid, &msg, sizeof(msg.text), 0) == -1){
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        
        if(msgrcv(Msgid, &buff, sizeof(mmsg.text), 2, 0)==-1) {
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
