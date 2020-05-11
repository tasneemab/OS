//
//  main.c
//  ex4a1
//
//  Created by Tasneem Abu Odeh on 02/12/2018.
//  Copyright © 2018 Tasneem Abu Odeh. All rights reserved.
//
//include(s) section
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

//Global(s) section
int msgid;

//Define(s) section
#define min 2
#define MAX_TEXT 200

//Struct(s) section
//This struct stores the data of the message.
struct Data {
    int pid;
    int num;
    char text[MAX_TEXT];
};

//This struct represents the message.
struct msgbuff{
    long mtype;
    char text[MAX_TEXT];
    struct Data data;
};

//Prototype(s) section
void signalHandler(int signum);
int buildMessagesQueue(void);
void primeFactors(char STR[MAX_TEXT], int num);
void addFactor(char STR[MAX_TEXT], int Val);
struct msgbuff server(struct msgbuff msg, int msgid);

//main section
int main(int argc, const char * argv[]) {
    struct msgbuff msg;
    signal(SIGINT, &signalHandler);
    msgid = buildMessagesQueue();
    
    for(;;)  {
        primeFactors(msg.text, atoi(msg.text));
        msg = server(msg, msgid);
    }
    
    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------
//A function to handle the SIGINT signal, it deletes the message queue.
void signal_handler(int signum){
    puts("Deleting message queue");
    if(msgctl(msgid,IPC_RMID,NULL) == -1){
        perror("msgctl() failed");
        exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------------------------
//A function to build a  messages queue.
int buildMessagesQueue(){
    key_t key;
    if((key=ftok(".", 'd'))==-1){
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }
    if((msgid = msgget(key, 0600 | IPC_CREAT | IPC_EXCL))==-1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
    return msgid;
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

//------------------------------------------------------------------------
//This function handles the app server, it receives messages from clients,
// sends messages to reg server to validate clients, calls the needed
// functions to perform what the client needs.
struct msgbuff server (struct msgbuff msg, int msgid) {
    
    struct msgbuff mmsg,buff;
    int i;
    mmsg.mtype = buff.mtype;
    //long type = 0;
    
    if(msgsnd(msgid, &msg, sizeof(msg.text), 0) == -1){
        perror("msgsnd failed:");
        exit(EXIT_FAILURE);
    }
    if(msgrcv(msgid, &buff, sizeof(mmsg.text),1, 0)==-1){
        perror("msgrcv failed:");
        exit(EXIT_FAILURE);
    }
    
    for(i=0; i<MAX_TEXT; i++)
        if(buff.text[i] == '\0')
            break;
        else
            mmsg.text[i]=buff.text[i];
            
            return mmsg;
}
