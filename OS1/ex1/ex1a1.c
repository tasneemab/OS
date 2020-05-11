#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h> 
#include <time.h>


void is_valid(int status);

int main()
{
	int i;
	int status;
	char *rings[5] = { "10", "20" ,"25", "30","35" };

	srand((unsigned)time(NULL));


	for (i = 0; i < 5; i++)
	{
		status = _spawnl(_P_NOWAIT, "ex1a2.exe", "ex1a2.exe", rings[i], NULL);
		is_valid(status);
	}
	
	getchar();

	return EXIT_SUCCESS;
}
//----------------------------------------
void is_valid(int status)
{
	if (status < 0)
	{
		perror("Cannot fork()");
		exit(EXIT_FAILURE);
	}
}
