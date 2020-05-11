#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h> 
#include <time.h>

void hanoi(int size, char src, char dest, char aux, FILE *fdr);

int main(int argc, char * argv[])
{
	FILE *fdr = fopen("nul", "w");
	if (!fdr)
	{
		perror("error with opening file");
		exit(EXIT_FAILURE);
	}

	time_t start, end;
	srand((unsigned)time(NULL));

	if (argc != 2) {
		fprintf(stderr, "Usage: %s ex1a1 argv[1]\n",
			argv[0]);
		getchar();
		exit(EXIT_FAILURE);
	}

	int size = atoi(argv[1]);

	start = time(NULL);
	hanoi(size, 'A', 'B', 'C', fdr);
	end = time(NULL);


	printf("%d = %d \n", size, (int)end - (int)start);


	fclose(fdr);
	getchar();
	return EXIT_SUCCESS;
}
//--------------------------------------------------------------------
void hanoi(int size, char src, char dest, char aux, FILE *fdr)
{
	if (size == 1)
	{
		fprintf(fdr, "\n Move disk 1 from rod %c to rod %c ", src, dest);
		return;
	}
	hanoi(size - 1, src, aux, dest, fdr);
	fprintf(fdr, "\n Move disk %d from %c to rod %c ", size, src, dest);
	hanoi(size - 1, aux, dest, src, fdr);
}