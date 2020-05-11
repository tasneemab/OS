#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <time.h>

void hanoi(int size, char from, char to, char aux, FILE *fptr);
void help(char * argv[], FILE *fptr);

int main(int argc, char * argv[])
{
    FILE *fptr;
    srand((unsigned)time(NULL));
    
    fptr = fopen("write.txt", "w");
    
    if(fptr == NULL ){
        perror("Cannot open file");
        exit(1);
    }
    
    if (argc != 2) {
        perror("Cannot open ex1a1");
        getchar();
        exit(1);
    }
    
    help(&argv[1], fptr);
    
    fclose(fptr);
    getchar();
    
    return EXIT_SUCCESS;
}
//--------------------------------------------------
//A helping function to call for the hanoi function.
void help(char * argv[], FILE *fptr){
    time_t start, end;
    int size = atoi(argv[1]);
    
    start = time(NULL);
    hanoi(size, 'A', 'B', 'C', fptr);
    end = time(NULL);
    
    
    printf("%d = %d \n", size, (int)end - (int)start);
}
//--------------------------------------------------
//A function to finds solution for .
void hanoi(int size, char from, char to, char aux, FILE *fptr)
{
    if (size == 1)
    {
        fprintf(fptr, "\n Move disk 1 from rod %c to rod %c ", from, to);
        return;
    }
    hanoi(size - 1, from, aux, to, fptr);
    fprintf(fptr, "\n Move disk %d from %c to rod %c ", size, from, to);
    hanoi(size - 1, aux, to, from, fptr);
}
