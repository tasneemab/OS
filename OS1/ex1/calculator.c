/*
 * ex1b.c
 *
 *  Created on: Nov 1, 2018
 *      Author: hananhi
 */

//includes section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//defines section
#define MAX_STR_LEN 20
#define N 100

//prototypes section
void do_plus ( );
void do_minus();
void do_sum ();
void do_max();
int gets_comand(char* temp );
void call_comand (int comand );

//main section
int main (int argc , char* argv[]){

	pid_t pid ;
	char *comand;
	int length =100 ;
	char delim[] = " ";
	char* token;
	char* first ;
	char**param ;

	while (fgets(comand, length , stdin)) {
		int i = 0 ;
		param = (char**)malloc(sizeof(char*));

		for (token = strtok(comand,delim); token != NULL ; token = strtok(NULL, delim)){

			param[0]= token ;
			i++;

			first = param[0];
			if ( strcmp("exit",first )){
				free(param);
				exit(EXIT_FAILURE);
			}

			if(gets_comand (first)== -1)
				perror("invalid command ");

			else {

				pid = fork();

				if (pid > 0 ){
					waitpid(pid,NULL,0);

				}

				if (pid== 0 ){
					call_comand(gets_comand(first));
				exit(EXIT_SUCCESS) ;
				}
			}



		}
	}
	return EXIT_SUCCESS;
}
//--------------------------
//A function to get the wanted command to be done
int gets_comand(char* temp ){

	int comand = -1 ;
	if (strcmp("plus",temp))
		comand = 0 ;

	if (strcmp("minus",temp))
		comand= 1 ;

	if (strcmp("sum",temp ))
		comand= 2 ;

	if (strcmp("max", temp))
		comand=3 ;

	return comand ;

}

//--------------------------
//Afunction to call the function that fits the wanted command
void call_comand (int comand ){

	switch(comand){
	case 0:
		do_plus ();
		break ;

	case 1:
		do_minus();
		break ;

	case 2 :
		do_sum ();
		break ;

	default:
		do_max ();
		break ;

	}
}

//--------------------------
//Afunction that do the plus command
void do_plus ( ){

	char *number ;

	char s1[MAX_STR_LEN], s2[MAX_STR_LEN];

	number= strtok(NULL ," ");
	if (number != NULL)
		strcpy(s1,number);

	number= strtok(NULL," ");
	if (number != NULL)
		strcpy(s2,number);

	char *args []={"./plus" , s1 ,s2 ,NULL};

	if (execvp ("plus", args)!= 0 ){
		perror (" execvp failed ");
		exit(EXIT_FAILURE);
	}

}

//--------------------------
//Afunction that do the minus command
void do_minus (){

	char *number ;

	char s1[MAX_STR_LEN], s2[MAX_STR_LEN];

	number= strtok(NULL ," ");
	if (number != NULL)
		strcpy(s1,number);

	number= strtok(NULL," ");
	if (number != NULL)
		strcpy(s2,number);

	char *args[] ={"./minus" , s1 ,s2 ,NULL};

	if (execvp ("minus",args)!= 0 ){
		perror (" execvp failed ");
		exit(EXIT_FAILURE);
	}
}

//--------------------------
//Afunction that do the sum command
void do_sum (){
	char my_string[N];
	char*number ;

	number = strtok(NULL,"/0");
	strcpy(my_string , number);

	if (execlp ("./sum","sum",my_string ,NULL) != 0 ){
		perror (" execlp failed ");
		exit(EXIT_FAILURE);
	}
}

//--------------------------
//Afunction that do the max command
void do_max(){

	int i = 1 ;

	 char **strings ;
	 char* word ;

	 strings = (char**)malloc(sizeof(char*));
	 char delim[] = "";


		 word = strtok(NULL, delim);
		 strings[0]= "max\0";
		 while(word != NULL){
			 strings = (char**)realloc(strings,sizeof(char*)* i+1 );
			 strings[i]= word ;
              i++ ;
              word = strtok(NULL, delim);
		 }

		 if (execvp ("max",strings)!= 0 ){
		 		perror (" execvp failed ");
		 		exit(EXIT_FAILURE);
		 	}

}
