//include(s) section.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
//#include <wait.h>

//Define(s) section.
#define NUMBERS 101
#define MINI 3

//Global(s) section.
int sharedMDad_id;
int sharedMSon1_id;
int sharedMSon2_id;
int sharedMSon_id;

//Prototype(s) sectiion.
int GCD(int num1, int num2);
void primeFactors(int num, int values[]);
void sonSharedMemory(key_t *key, int *shm_id, char type, const int size);
void deleteSharedMemory(int shm_id);
void sharedMemorySib(key_t *key, char type, int *shm_id);
void checkSharedMemory(int *ptr);
void fatherSharedMemory(key_t *key);
void printFactors(int *values);
void checkson(pid_t status);
void SignalHandler(int signum);
void signalHandler1(int sig);
void signalHandler2(int sig);
void sonProcess(int i, pid_t *son_id);
void sonProcess1(pid_t *son_id);
void sonProcess2(pid_t *son_id);
void sonProcess3(pid_t *son_id);
void reset(int values[]);

//main section.
int main(){
    int i;
    key_t key;
    pid_t status, *son_id;
    
    signal(SIGINT, SignalHandler);
    
    fatherSharedMemory(&key);
    sharedMSon_id = sharedMDad_id;
    
    son_id = (pid_t *) shmat(sharedMDad_id, NULL, 0);
    checkSharedMemory(son_id);
    
    for(i = 0; i < 3; i++){
        status = fork();
        
        if(status == 0){
            son_id[i]= getpid();
            sonProcess(i, son_id);
            exit(EXIT_SUCCESS);
        }
        checkson(status);
        
    }
    
    for(i = 0 ; i < MINI; i++)
        wait(NULL);
    
    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
//A function that catches the siganl (SIGINT).
void SignalHandler(int signum){
    printf("process %d got SIGINT\n", (int)getpid());
    deleteSharedMemory(sharedMSon_id);//frees the memory we've been sharing.
    exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------
//A function that checks if the fork process's successed.
void checkson(pid_t status){
    if(status < 0){
        perror("Cannot fork:");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------
//A function that finds the GCD for two given numbers.
int GCD(int num1, int num2){
    int remainder = num1 % num2;
    
    if (remainder == 0) {
        return num2;
    }
    
    return GCD(num2, remainder);//calls the function in recursive to find the GCD
}

//----------------------------------------------------------------------------
//A function that find the prime factors of a given number.
void primeFactors(int num, int *values){
    int i;
    int j;
reset(values);//to fill the array with zeros.
    i = 1;
    while(num%2 == 0){
        values[i] = 2;
        i++;
        num = num/2;
    }
    for(j = 3; j < (num/2); j += 2){
        
        while(num%j == 0){
            values[i] = j;
            i++;
            num = num/j;
        }
    }
    if(num > 2)
        values[i] = num;

}
//---------------------------------------------------------------------------
//A function to fill the array with zeros.
void reset(int values[]){
    int i;
    for(i = 1; i < NUMBERS; i++)
        values[i] = 0;
}
//---------------------------------------------------------------------------
//A function that check out for the sons.
void sonProcess(int i, pid_t *son_id){
    switch(i){
        case 0:
            sonProcess1(son_id);
            break;
        case 1:
            sonProcess2(son_id);
            break;
        case 2:
            sonProcess3(son_id);
            break;
    }
}
//---------------------------------------------------------------------------
//A function that  passes us to the GCD server.
void sonProcess1(pid_t *son_id){
    key_t key1;
    int *values;
    
    
    signal(SIGUSR1, signalHandler1);
    sonSharedMemory(&key1, &sharedMSon1_id, 'g', 3);
    
    values = (int *) shmat(sharedMSon1_id, NULL, 0);
    checkSharedMemory(values);
    
    while(1){
        pause();
        values[2]= GCD(values[0], values[1]);
        kill(son_id[2],SIGUSR2);
    }
}
//---------------------------------------------------------------------------
//A function that passes us to the prime factors server.
void sonProcess2(pid_t *son_id){
    key_t key2;
    int *numbers;
    
    signal(SIGUSR1, signalHandler1);
    sonSharedMemory(&key2, &sharedMSon2_id, 'd', NUMBERS);
    
    numbers = (int *) shmat(sharedMSon2_id, NULL, 0);
    checkSharedMemory(numbers);
    
    while(1){
        pause();
        primeFactors(numbers[0],numbers);
        kill(son_id[2],SIGUSR2);
    }
}
//---------------------------------------------------------------------------
//A function that passes us to the client server.
void sonProcess3(pid_t *son_id){
    char type;
    int num1, num2;
    key_t key1, key2;
    int *values;
    
    signal(SIGUSR2, signalHandler2);
    
    sleep(1);
    sharedMemorySib(&key1, 'g', &sharedMSon1_id);
    
    sharedMemorySib(&key2, 'd', &sharedMSon2_id);
    
    while(1){
        sleep(1);
        puts("Choose which peocess g or d:");
        scanf(" %c", &type);
        if(type == 'g'){
            values =  (int *) shmat(sharedMSon1_id, NULL, 0);
            checkSharedMemory(values);
            puts("Please enter two numbers:");
            scanf("%d %d", &num1, &num2);
            values[0] = num1;
            values[1] = num2;
            kill(son_id[0], SIGUSR1);
            pause();
            printf("The GCD is %d\n", values[2]);

        }
        if(type == 'd'){
            values =  (int *) shmat(sharedMSon2_id, NULL, 0);
            checkSharedMemory(values);
            puts("Please enter one number:");
            scanf("%d", &num1);
            values[0] = num1;
            kill(son_id[1],SIGUSR1);
            pause();
            printFactors(values);
        }
    }
}

//---------------------------------------------------------------------------
//A function that prints out the prime factors of a given number.
void printFactors(int *values){
    int j;

    printf("The Prime factors of the choosen number are:\n");
    for(j = 0; j < NUMBERS -1; j++){
        if(values[j+1] == 0)
            break;
        printf("%d ", values[j+1]);
    }
    printf("\n");
    
}

//---------------------------------------------------------------------------
//A function that creates the parent shared memory.
void fatherSharedMemory(key_t *key){
    *key = ftok(".", 'f');
    if(*key == -1){
        perror("Error in ftok: ");
        exit(EXIT_FAILURE);
    }
    if((sharedMDad_id = shmget(*key, 3*sizeof(pid_t),IPC_CREAT | IPC_EXCL | 0600)) == -1){
        perror("shmget of dad's shared memory has been failed:");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------
//A fuction that combines to a son shared memory.
void sharedMemorySib(key_t *key, char type, int *shm_id){
    *key = ftok(".", type);
    if(*key == -1){
        perror("Error in ftok:");
        exit(EXIT_FAILURE);
    }
    if((*shm_id = shmget(*key, 0,0600)) == -1){
        perror("Error in shmget: ");
        exit(EXIT_FAILURE);
    }
    
}
//---------------------------------------------------------------------------
//A function that creates  sonren shared memory.
void sonSharedMemory(key_t *key, int *shm_id, char type, int size){
    *key = ftok(".", type);
    if(*key == -1){
        perror("Error in ftok: ");
        exit(EXIT_FAILURE);
    }
    if((*shm_id = shmget(*key,size*sizeof(int),IPC_CREAT | IPC_EXCL | 0600)) == -1){
        perror("Error in shmget: ");
        exit(EXIT_FAILURE);
    }
    sharedMSon_id = *shm_id;
}

//---------------------------------------------------------------------------
//A function that frees the shared memory.
void deleteSharedMemory(int shm_id){
    if (shmctl(shm_id,IPC_RMID, NULL) == -1) {
        perror( "shmctl failed" ) ;
        exit( EXIT_FAILURE ) ;
    }
}
//---------------------------------------------------------------------------
//A function that checks if shared memory creation was successed.
void checkSharedMemory(int *ptr){
    if(ptr == NULL){
        perror("Error in shmat: ");
        exit(EXIT_FAILURE);
    }
}
//---------------------------------------------------------------------------
void signalHandler1(int sign){
    
}
//---------------------------------------------------------------------------
void signalHandler2(int sign){
    
}
