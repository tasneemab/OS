/**
 * plus : the sum of two numbers  .
 *====================================================================
 * Written by: hananhi , Id: 318314663 , login: hananhi
 *              Tasneem Abu Odeh, 314902685, tasnimab
 *
 *The program recevied twonumber and calculate the sum of them
 *
 *input : two numbers
 *output: the sum of the numbers
 */

//==================include section===========
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//============function declration ===========
int isNumber(char * str);

//main function
int main (int argc ,char * argv []){
    int sum_of_num ;
    if(argc != 3){
    	puts("wrong usage");
    	exit(EXIT_FAILURE);
    }

    if (!isNumber(argv[1]) || !isNumber(argv[2])){
    	puts("error");
    	exit(EXIT_FAILURE);
    }


    sum_of_num = atoi(argv[1]) + atoi(argv[2]) ;
    printf("%d\n",sum_of_num);

    exit(EXIT_SUCCESS);
}


//a function to finds if the input is number
int isNumber(char * str){
	int i ;

	for(i = 0 ; i <  strlen(str); i++){
		if(str[0] == '-' && strlen(str) > 2)
			continue;
		if(!isdigit(str[i]))
			return 0;
	}
	return 1;
}
