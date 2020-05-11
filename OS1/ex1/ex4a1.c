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
#include <string.h>

//Global(s) section
int msgid;

//Define(s) section
#define min 2
#define MAX_LEN 200

//Struct(s) section
//This struct stores the data of the message.
struct Data {
    int pid;
};

//This struct represents the message.
struct msgbuff{
    long mtype;
    int num1, num2;
    char text[MAX_LEN];
    struct Data data;
};

//Prototype(s) section.
void signalHandler(int signum);
int buildMessagesQueue(void);
int initRegServer(void);
int GCD(int num1, int num2);
struct msgbuff server(struct msgbuff msg, int msgid);
void extractNumbers(char STR[MAX_LEN], int *num1, int *num2);
void adding(char STR[MAX_LEN], int num);
struct msgbuff recMsg(int msgid);
void sendMsg(int msgid, struct msgbuff msg);

//main section.
int main(int argc, const char * argv[]) {
    
    int num1, num2;
    struct msgbuff msg;
    
    signal(SIGINT, &signalHandler);
    msgid = buildMessagesQueue();
    
   while(1){
       
       extractNumbers(msg.text, &num1, &num2);
        adding(msg.text, GCD(num1, num2));
        msg = server(msg, msgid);
    }
    
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//A function that handles the SIGINT signal, it deletes the message queue.
void signalHandler(int signum){
    puts("Deleting messages queue");
    if(msgctl(msgid,IPC_RMID,NULL) == -1){
        perror("msgctl() failed");
        exit(EXIT_FAILURE);
    }
    
}

//--------------------------------------------------------
//A function that initializes the messages queue.
// it returns the id of the queue.
int buildMessagesQueue(){
    key_t key;
    if((key = ftok(".",'g')) == -1){
        fprintf(stderr, "ftok(0) failed!\n");
        exit(EXIT_FAILURE);
    }
    if((msgid = msgget(key,IPC_CREAT | IPC_EXCL | 0600)) == -1){
        fprintf(stderr, "msgget failed!\n");
        exit(EXIT_FAILURE);
    }
    return msgid;
}

//--------------------------------------------------------
//A function that finds the GCD for two numbers.
int GCD(int num1, int num2){
    int remainder = num1 % num2;
    
    if (remainder == 0) {
        return num2;
    }
    
    return GCD(num2, remainder);
}

//--------------------------------------------------------
//A function to extract numbers.
void extractNumbers(char STR[MAX_LEN], int *num1, int *num2){
    char *ptr = STR;
    *num1 = strtol(ptr, &ptr, 10);
    ptr++;
    *num2 = strtol(ptr, &ptr, 10);
}

//--------------------------------------------------------
//A function to add values.
void adding(char STR[MAX_LEN], int num){
    memset(STR, 0, sizeof(char)*MAX_LEN);
    sprintf(STR, "%d", num);
}
//--------------------------------------------------------
//This function handles the app server, it receives
// messages from clients,sends messages to reg server to
// validate clients, calls the neededfunctions to perform what
// the client needs.
struct msgbuff server(struct msgbuff msg, int msgid){
    
    struct msgbuff mmsg, buff;
    int i;
    
    if(msgsnd(msgid, &msg, sizeof(msg.text), 0) == -1){
        perror("msgsnd failed:");
        exit(EXIT_FAILURE);
    }
    
    if(msgrcv(msgid, &buff, sizeof(mmsg.text), 1, 0)==-1){
        perror("msgrcv failed:");
        exit(EXIT_FAILURE);
    }
    
    mmsg.mtype=buff.mtype;
    
    for(i=0; i<MAX_LEN; i++)
        if(buff.text[i]=='\0')
            break;
        else
            mmsg.text[i]=buff.text[i];
            
            return mmsg;
}

