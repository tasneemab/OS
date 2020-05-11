/**
 * plus : the sum of two numbers  .
 *====================================================================
 * Written by: hananhi , Id: 318314663 , login: hananhi
 *        Tasneem Abu Odeh, 314902685, tasnimab
 *
 *The program recevied numbers and finds the maximum
 *
 *input : numbers
 *output: the maximum number
 */

//include section
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

//main function
int main (int argc , char **argv){
	int max_num;
	int i,j;

	for(i = 1,j=0; i< argc;i++){
		while(argv[i][j] != '\0'){
			if(!isdigit(argv[i][j])){
				perror("input is not currect");
				exit(EXIT_FAILURE);
			}
			j++;
		}
	}

	max_num = atoi(argv[1]);

	for (i=2; i < argc; i++  ){
		if(atoi(argv[i]) >= max_num){
			max_num = atoi(argv[i]);
		}

	}
	printf("%d\n" , max_num);

	return EXIT_SUCCESS ;
}
