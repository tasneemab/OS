
/* ex4c
 * written by : Jouana Nusralla --  login: jouananu --   id:312315104
 *                 Tasnim abu ohde -- login tasnima -- id:314902685
 *
 * The parent process assigns shared memory and make two child processes,
 * each of the children in a loop choose randomly number and fills
 * their array. each time they take same number from the dad(generated randomly)
 * and check if it is in their array. the child which finds all his array numbers
 * kills his dad and brother and print its array.
 * Q answer: if both children find the last number in the same while then they both
 *              wins. then both of them prints their arrays and kills the dad and each
 *              other. this is "Race mode"
 */

//include(s) section.
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/msg.h>
#include  <stdbool.h>
#include  <signal.h>
#include  <unistd.h>
#include  <time.h>
#include <sys/wait.h>
#include <sys/shm.h>

//Define(s) section.
#define UPPER 100
#define LOWER 1

//Prototype(s) section.
void do_child1();
void do_child2();
void signal_handler_user1(int sig_num);
void signal_handler_user2(int sig_num);
//returns the place of the number if found or -1
int find_number(int number,int arr[],int arr_bool[]);
//prints array
void print_arr(int arr[]);


//Struct(s) section.
typedef struct S
{
    int pid;
    int num;
}S;

//Global(s) section.
int firstch , secondch ;// two sons
S *parent_pointer;
int shm_id;
int ch1_arr[10]={0};
int ch2_arr[10]={0};

//main section.
int main()
{
    int x;
    key_t key;
    srand(time(NULL));
    
    //shared memory
    key = ftok(".",'f');
    if((shm_id = shmget(key,1000,IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        perror("failed in shmget1");
        exit(1);
    }
    if((parent_pointer = (S*)shmat(shm_id,NULL,0))<0)
    {
        perror("failed in shmat");
        exit(1);
    }
    
    signal(SIGUSR2, signal_handler_user2) ;
    firstch = fork();     //create the first son
    
    if (firstch < 0)
    {
        printf(" Error fork \n");
        exit(EXIT_FAILURE);
    }
    
    else if (firstch == 0)
    {
        signal(SIGUSR2,signal_handler_user2);
        parent_pointer[0].pid = getpid();
        do_child1();
        exit(EXIT_SUCCESS);
    }
    //create second child
    secondch = fork();
    if (secondch < 0)
    {
        printf("*** fork error ***\n");
        exit(EXIT_FAILURE);
    }
    else if (secondch == 0)
    {
        signal(SIGUSR2,signal_handler_user2);
        parent_pointer[1].pid = getpid();
        do_child2();
        exit(EXIT_SUCCESS);
    }
    
    signal(SIGUSR1, signal_handler_user1) ;
    srand(time(NULL));
    parent_pointer[2].num = 0;
    parent_pointer[3].num = 0;
    while(1)
    {
        if((parent_pointer[2].num == 0)&&
           (parent_pointer[3].num == 0))
        {
            //parent random number in range
            x = (rand() %
                 (UPPER - LOWER + 1))
            + LOWER;
            parent_pointer[2].num = x;
            parent_pointer[3].num = x;
        }
        
    }
    return EXIT_SUCCESS;
}

//----------------------------------------------------------------------
/*
 * this function fills child1 array with random numbers, and each time
 * takes from the dad using shared memory the number dad put and checks if it is
 * in his array. when he receives from his dad all his array numbers
 * he prints the array and kills his dad and the dad kill the second child.
 */
void do_child1()
{
    int i, sum=0, child1_id;
    int ch1_bool[10]={0};
    key_t key;
    S *ch1_ptr;
    key = ftok(".",'f');
    srand(time(NULL)+1);
    //fill the array with random numbers
    for(i=0;i<10;i++)
    {
        //random number in range
        ch1_arr[i] = (rand() %
                      (UPPER - LOWER + 1))
        + LOWER;
    }
    if((child1_id = shmget(key,0,0600)) == -1)
    {
        perror("failed in shmget2");
        exit(1);
    }
    if((ch1_ptr = (S*)shmat(child1_id,NULL,0))<0)
    {
        perror("failed in shmat");
        exit(1);
    }
    
    while(1)
    {
        //my dad already put new number for me
        if(ch1_ptr[2].num != 0)
        {
            if(find_number(ch1_ptr[2].num,ch1_arr,ch1_bool)>=0)
            {
                //the number were found
                sum++;
            }
            //reset num for resubmitting by the dad
            ch1_ptr[2].num = 0;
        }
        if(sum == 10)
        {
            printf("first child is the winner\n");
            print_arr(ch1_arr);
            kill(getppid(),SIGUSR1);
            exit(0);
        }
    }
    
}
//----------------------------------------------------------------------
/*
 * same as so_child1() function
 */
void do_child2()
{
    int i, sum=0,child2_id;
    int ch2_bool[10]={0};
    key_t key;
    key = ftok(".",'f');
    S *ch2_ptr;
    srand(time(NULL)+2);
    //fill the array with random numbers
    for(i=0;i<10;i++)
    {
        //random number in range
        ch2_arr[i] = (rand() %
                      (UPPER - LOWER + 1))
        + LOWER;
    }
    if((child2_id = shmget(key,0,0600)) == -1)
    {
        perror("failed in shmget3");
        exit(1);
    }
    if((ch2_ptr = (S*)shmat(child2_id,NULL,0))<0)
    {
        perror("failed in shmat");
        exit(1);
    }
    
    while(1)
    {
        //my dad already put new number for me
        if(ch2_ptr[3].num != 0)
        {
            if(find_number(ch2_ptr[3].num,ch2_arr,ch2_bool)>=0)
            {
                //the number were found
                sum++;
            }
            //reset num for resubmitting by the dad
            ch2_ptr[3].num = 0;
        }
        if(sum == 10)
        {
            printf("second child is the winner\n");
            print_arr(ch2_arr);
            kill(getppid(),SIGUSR1);
            exit(0);
        }
    }
    
}

//---------------------------------------------------------------------
/*
 * return number place in array if if was found
 */
int find_number(int number,int arr[],int arr_bool[])
{
    int i;
    for(i=0;i<10;i++)
    {
        if(arr[i]==number && arr_bool[i]==0)
            return i;
    }
    return -1;
}
/*
 * function that prints array members.
 */
void print_arr(int arr[])
{
    int i;
    for(i=0;i<10;i++)
    {
        printf(" %d",arr[i]);
    }
}
void signal_handler_user1(int sig_num)
{
    kill(parent_pointer[0].pid,SIGUSR2);
    kill(parent_pointer[1].pid,SIGUSR2);
    if(shmctl(shm_id,IPC_RMID, NULL) == -1)
    {
        perror("shmctl() failed");
        exit(EXIT_FAILURE);
    }
    exit(0);
}


void signal_handler_user2(int sig_num)
{
    exit(0);
}
