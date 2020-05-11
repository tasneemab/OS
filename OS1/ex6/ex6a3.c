/**
 * Pure C 2019
 * ex6a3.c
 *  ex6a3
 *
 *   Created by Tasneem Abu Odeh - id: 314902685 - login: tasnimab.
 *             Jouana Nusralla - id:312315104 - login: jouananu.
 *
 *
 *  This is a client program that connect two servers
 * one to check the gcd the another to check prime factor
 * when the program connect the two servers.
 * if enter 0 the program want another to numbers
 * to check the gcd and send the data to the correct server.
 * if enter 1 the program want another number to check the
 * prime of it then send it to the correct server
 * if enter 2 thats means to close the program and exit
 * from the servers.
 * input :
 * 1) 0 : to check the gcd of two numbers
 * then the two numbers wanted to check
 * 2) 1 : to check the factor prime of two numbers
 * then the number wanted to check
 * 3) 2 : to close the program
 * output : results on the standard output.
 *
 *  Copyright © 2018 Tasneem Abu Odeh. All rights reserved.
 */
//---------------------------------------------------------------

//include(s) section.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                     // for read/write/close
#include <sys/types.h>                  // standard system types
#include <netinet/in.h>                 // Internet address structures
#include <sys/socket.h>                 //socket interface functions
#include <netdb.h>                      // host to IP resolution
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>
#include <fcntl.h>

//Global(s) section.
struct addrinfo conKind, *addrInfoRes1, *addrInfoRes2;
int GCDsocket, PFsocket;
int command;

//Define(s) section.
#define GCD 0
#define PRIME_FACTORS 1
#define OVER 2
#define CLIENTS 5
#define NUMS 100

//Prototype(s) section.
int checkSocket(struct addrinfo ** addrInfoRes);
void errorEccurred();
void clearAllocations(void);
void runClientServer(const char * argv[]);
void sendData(int socketName);
void printResult(int PF[]);
void connectServer(const int socketName, const struct addrinfo * addrInfoRes);
void getAddrInfo(const char * IP_Address, const char * MyPort, struct addrinfo * conKind,
                 struct addrinfo ** addrInfoRes);

//main section.
int main(int argc, const char * argv[]) {
    if(argc != 5){
        fprintf(stderr, "Missing Server name \n");
        exit(EXIT_FAILURE);
    }
    
    runClientServer(argv);
    
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------
//This functions run the client server.
void runClientServer(const char * argv[]){
    
    //declerate all the needed variables in this scope
    const char * GCD_IP = argv[1];
    const char * PF_IP = argv[3];
    const char * GCD_PORT = argv[2];
    const char * PF_PORT = argv[4];
    
    int PF[NUMS], number;
    int GCDinput[OVER], GCDresult;;
    
    int rc1;
    int rc2;
    
    memset(&conKind, 0, sizeof(conKind));
    //set the conKind variables
    conKind.ai_family = AF_UNSPEC;
    conKind.ai_socktype = SOCK_STREAM;
    
    //get the address information of each server
    getAddrInfo(GCD_IP, GCD_PORT, &conKind, &addrInfoRes1);
    getAddrInfo(PF_IP, PF_PORT, &conKind, &addrInfoRes2);
    
    //opens socket & checks if it sucesseded
    GCDsocket = checkSocket(&addrInfoRes1);
    PFsocket = checkSocket(&addrInfoRes2);
    
    //connecting to the server
    connectServer(GCDsocket, addrInfoRes1);
    connectServer(PFsocket, addrInfoRes2);
    
    //main engine of client
    while(1){
        puts("Choose which command you want to do:\n 0, 1 or 2!");
        scanf("%d", &command)
        //passes the client to the GCD server
        if(command == GCD){
            scanf("%d%d", &GCDinput[0], &GCDinput[1]);//gets tow numbers
            write(GCDsocket, &GCDinput, sizeof(GCDinput));//check the sizeof
            rc1 = read(GCDsocket, &GCDresult, sizeof(GCDresult));
            if(rc1 > 0){ //print out the GCD of two entered numbers
                printf("The GCD of the two numbers you've entered %d & %d is : %d.\n", GCDinput[0], GCDinput[1], GCDresult);
            }
            else//something went wrong
                errorEccurred();
            
        }
        //passes the client to the prime factors server
        else if(command == PRIME_FACTORS){
            scanf("%d", &number);//gets a number
            write(PFsocket, &PF, sizeof(int));//check the sizeof
            sleep(1);//give time for the server
            rc2 = read(PFsocket, &Pf, sizeof(PF));
            if(rc2 > 0){//print out the prime factors of the entered number
                printf("The prime factors for number %d are : ", number);
                printResult(PF);
            }
            else // something went wrong
                errorEccurred();
            
        }//exits the server
        else if(command == OVER)
            break;
        else
            continue;
    }
    
    clearAllocations();
    
}
//---------------------------------------------------------------
//This function frees all the data and allocation after the client finish his process.
void clearAllocations(void){
    
    //disconnect the servers
    close(GCDsocket);
    close(PFsocket);
    
    //delete the allocated address
    freeaddrinfo(addrInfoRes1);
    freeaddrinfo(addrInfoRes2);
}

//---------------------------------------------------------------
//This function connects the client to the server.
void connectServer(const int socketName, const struct addrinfo * addrInfoRes){
    int rc;
    rc = connect(socketName, addrInfoRes->ai_addr, addrInfoRes->ai_addrlen);
    if(rc){
        perror("connectin failed :");
        exit(1);
    }
}

//---------------------------------------------------------------
//This function get the address information.
void getAddrInfo(const char * IP_Address, const char * MyPort, struct addrinfo * conKind,
                 struct addrinfo ** addrInfoRes){
    
    int rc;
    rc = getaddrinfo(IP_Address, MyPort, (*& conKind), (*& addrInfoRes));
    if( rc != 0){
        fprintf(stderr, "getaddrinfo() failed %s \n", gai_strerror(rc));
        exit(1);
    }
}

//---------------------------------------------------------------
//This function gets a socket ID & the struct of socket details,Checks if the
//socket opens successfully.
int checkSocket(struct addrinfo ** addrInfoRes){
    int socketName = socket((*addrInfoRes) -> ai_family,
                            (*addrInfoRes) -> ai_socktype,
                            (*addrInfoRes) -> ai_protocol);
    
    if(socketName < 0){
        perror("Socket allocation failed :");
        exit(1);
    }
    return socketName;
}

//---------------------------------------------------------------
//This function send the data to the server.
void sendData(int socketName){
    size_t input;
    scanf("%zd", &input);
    
    if(write(socketName, &input, sizeof(int)) != sizeof(int)){
        perror("Socket writing failed :");
        exit(1);
    }
}

//---------------------------------------------------------------
//This function prints out the data on the screen.
void printResult(int  PF[]){
    int index = 0;
    
    for(index = 0; PF[index] != -1; index++){
        printf("%d ", factor[index]);
    }
    puts("\n");
}
//---------------------------------------------------------------
//This function prints out error message.
void errorEccurred(void){
    perror("Socket reading failed :");
    exit(1);
}
