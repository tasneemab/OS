#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <time.h>

#define  char *rings[5] = { "10", "20" ,"25", "30","35" }

void isValid(int status);

int main()
{
    int index;
    int status;
    
    srand((unsigned)time(NULL));
    
    
    for (index = 0; index < 5; index++){
        status = _spawnl(_P_NOWAIT, "ex1a2.exe", "ex1a2.exe", rings[index], NULL);
        isValid(status);
    }
    
    getchar();
    
    return EXIT_SUCCESS;
}
//----------------------------------------
void isValid(int status)
{
    if (status < 0)
    {
        perror("Cannot fork()");
        exit(1);
    }
}
