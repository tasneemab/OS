/**
 * plus : the sum of two numbers  .
 *====================================================================
 * Written by: hananhi , Id: 318314663 , login: hananhi
 *        Tasneem Abu Odeh, 314902685, tasnimab
 *
 *The program recevied two numbers and calculate the sum of them
 *
 *input : two numbers
 *output: the sum of the numbers
 */

//includes section
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int main(int argc, char *argv[]) {

	int number,temp;//to save the new numbers
	int index = 0;//to points the string
	int counter = 0;//save the numbers of each number
	int sign =0;//to save if the number is neg or pos
	int num_of_numbers = 0;//to save the number of numbers in the string
	int *numbers = malloc(sizeof(int*) * 0);//to save the numbers
	int sum = 0;
	//check the arguments
	if (argc != 2) {
			fprintf(stderr, "%s", " can not open the input file \n ");
			exit(EXIT_FAILURE);
	}
	//check if the array have characters
	while(argv[1][index] != '\0'){
		if(argv[1][index] != '-' && !isdigit(argv[1][index]) && argv[1][index] != ' '){
			fprintf(stderr, "%s", " the input is not a digit\n ");
			exit(EXIT_FAILURE);
		}
		index++;
	}

	//to convirt the numbers to integer in dynamic array
	for(index =0;argv[1][index] != '\0';index++){
		counter =0;
		sign = 0;
		//find each nuber in the string
		while(argv[1][index] != ' '){
			//to check if the number is neg.
			if(argv[1][index] == '-'){
				sign = 1;//the number is neg.
				index++;
				continue;
			}
			temp = argv[1][index] - '0';

			if(counter == 0){
				number = temp ;
				index++;
				counter ++;
				continue;
			}
			else{
				number = number*10;
				number += temp;
				index++;
				counter ++;
				continue;
			}
		}
		if(sign == 1){
			number = number * (-1);
		}
		num_of_numbers ++;
		numbers = (int *) realloc(numbers, num_of_numbers);
		if(numbers == NULL){
			perror("cannot allocate memory");
			exit(EXIT_FAILURE);
		}
		numbers[num_of_numbers-1] = number;
	}
	for(index = 0;index < num_of_numbers;index++){
		sum += numbers[index];
	}
	printf("%d\n", sum);
	exit(EXIT_SUCCESS);
}

