/**
 * Pure C 2019
 * ex6a2.c
 *  ex6a2
 *
 *   Created by Tasneem Abu Odeh - id: 314902685 - login: tasnimab.
 *             Jouana Nusralla - id:312315104 - login: jouananu.
 *
 *
 *   This Program used as the REGISTRY SERVER.
 * this server open a socket for clients and allows to check the
 * prime factors of a number
 * when the program get the number that the client want to check
 * the server get the factor and send it back..
 * this server can treat allot of users in the same time..
 * the server took the port number from the arguments string ..
 * input: from he client
 * output: to the client
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
struct addrinfo conKind, *addrInfoRes;
int mainSocket, servingSocket;

//Define(s) section.
#define NUMS     100
#define GCD 0
#define PRIME_FACTORS 1
#define SIZE 2
#define CLIENTS 5

//Prototype(s) section.
int checkSocket(struct addrinfo ** addrInfoRes);
int getGCD(int Val1, int Val2);
void getPrimeFactors(int factors[], int Val);
void signalHandler(int signum);
void runPFServer(const char * MyPort);
void bindSocket(const int * socketName, struct addrinfo ** addrInfoRes);
void listenSocket(const int * socketName);
void getAddrInfo(const char * IP_Address, const char * MyPort, struct addrinfo * conKind,
                 struct addrinfo ** addrInfoRes);


//main section.
int main(int argc, const char * argv[]) {
    
    //Checks the the arguments
    if(argc != 2){
        perror("arguments are incorrect");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, signalHandler);    //signal handler call
    runPFServer(argv[1]);//runs the server
    
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------
//This function runs the GCD server.
void runPFServer(const char * MyPort){
    //declare all the needed variables in this scope
    fd_set rfd, c_rfd;
    size_t status;
    int fd;
    int input;
    int PF[NUMS];
    
    
    memset(&conKind, 0, sizeof (conKind)) ; // set conKind  vars.
    conKind.ai_family = AF_UNSPEC ;
    conKind.ai_socktype = SOCK_STREAM ;
    conKind.ai_flags = AI_PASSIVE ;        // system will fill my IP
    
    getAddrInfo(NULL, MyPort, &conKind, &addrInfoRes); //get address information
    mainSocket = checkSocket(&addrInfoRes); //opens socket & checks if it sucesseded
    bindSocket(&mainSocket, &addrInfoRes); //bind the main socket
    listenSocket(&mainSocket); //start listenningon it
    
    FD_ZERO(&rfd); //reset the main socket
    FD_SET(mainSocket, &rfd); //set the main socket
    
    while(1){
        c_rfd = rfd; //saves a copy of the rfd
        
        select(getdtablesize(), &c_rfd, NULL, NULL, NULL); //selects the waiting clients
        
        if (FD_ISSET(mainSocket, &c_rfd)) {
            servingSocket = accept(mainSocket, NULL, NULL);
            if (servingSocket >= 0) {
                FD_SET(servingSocket, &rfd);
            }
        }
        for (fd= mainSocket +1; fd < getdtablesize(); fd++)
            if (FD_ISSET(fd, &c_rfd)) {
                
                status = read(fd, &input, sizeof(int)); //reads from the socket
                if (status == 0) {
                    close(fd);
                    FD_CLR(fd, &rfd);
                }
                else if (status > 0) {
                    getPrimeFactors(PF, input);
                    write(fd , &PF, sizeof(PF));
                }
                else {
                    perror("Socket reading failed") ;
                    exit(EXIT_FAILURE) ;
                }
            }
    }
}
//---------------------------------------------------------------
//This function finds the prime factors for a given value.
void getPrimeFactors(int factors[NUMS], int Val){
    int index;
    int div;
    
    //printf("\nThe prime factors of %d are: \n\n",number);
    
    index = 0;
    div = 2;
    
    while(Val!=0){
        if(Val%div!=0)
            div = div + 1;
        else {
            Val = Val / div;
            factors[index] = div;
            index++;
            if(Val == 1)
                break;
        }
    }
    //get the correct length data
    factors[index] = -1;
}
}

//---------------------------------------------------------------
//This function handles the SIGINT signal, it shuts down the server.
void signalHandler(int signum){
    puts("The server is shutting down right now!");
    freeaddrinfo(addrInfoRes);
    
    exit(0);
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
//This function gets a socket name & its details, performs bind.
void bindSocket(const int * socketName, struct addrinfo ** addrInfoRes ){
    if (bind((*socketName), (*addrInfoRes) -> ai_addr,
             (*addrInfoRes) -> ai_addrlen)){
        perror("Socket binding failed :");
        exit(1);
    }
}

//---------------------------------------------------------------
// This function gets a socket name & its details,performs listen.
void listenSocket(const int * socketName){
    if(listen((*socketName), CLIENTS)){
        perror("Socket listenning failed :");
        exit(1);
    }
    
}

//---------------------------------------------------------------
// This function gets a server IP & port ,and 2 structs of his socket details,
// check if it got the info.
void getAddrInfo(const char * IP_Address, const char * MyPort, struct addrinfo * conKind,
                 struct addrinfo ** addrInfoRes){
    int rc;
    rc = getaddrinfo(IP_Address, MyPort, (*& conKind), (*& addrInfoRes));
    if( rc != 0){
        fprintf(stderr, "getaddrinfo() failed %s \n", gai_strerror(rc));
        exit(1);
    }
}

