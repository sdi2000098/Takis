#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"


int commands ( char ** words, int tabs , char * argv)			/* A function that deals with the commands. */
{						/* Getting as arguments a line after words are seperated, the number of tabs that have to exist in this loop, argv is used for the '-v' mode */
	extern struct var * struct_arr ;
	extern struct ipl_arrays ** all_arrays ;
	extern struct double_num * ipl_double;
	extern int array_size,break_count,continue_count,size ,double_size;
	int i=0,j=0,k=0,condition,count_tabs=0;
	double x;
	char ch[100];
	struct valid temp; 
	if ( words[0] == NULL ) {		/* This means that the line is empty or full of comments */
		return 0;
	}
	else {
		while (words[0][k++]=='\t'){	/* Count tabs */
			i=1;
			count_tabs++;
		}
	}
	if ( count_tabs  > tabs ){
		printf("Too many tabs\n");
		return ERROR;
	}
	if ( strcmp(argv,"-v") == 0 ){		/* Need to print what is read */
		int temp;
		for ( temp=0;temp<count_tabs;temp++){		/* Print the amount of tabs */
			putchar('\t');
		}
		temp=i;
		while ( words[temp] != NULL ){			/* Print each word, seperated by one spacebar */
			printf("%s ",words[temp]);
			temp++;
		}
		putchar('\n');
	}
	if ( strcmp(words[i],"read") == 0 ){
		i++;
		if ( check_var(words[i]) == ERROR ){
			printf ("Invalid name for variable \n");
			return ERROR;
		}
		else if ( check_var( words[i] ) == ITS_ARRAY ){
			if ( commands_array(words[i],"read",666,NULL) == ERROR){
				return ERROR;
			}
		}
		else{
			if ( scanf("%lf",&x) != 1 ){		/* Read the number as double */
				printf("Expected integer or decimal\n");
				return ERROR;
			}
			if ( ( k = find_double(words[i]) )!= double_size ){	/* There is already a double variable with such name */
				ipl_double[k].x = x;
			}
			else if ( ( k=find_var(words[i]) ) != size ){
				if ( x != (int)x ){		/* Means that the number read is not type int */
					printf("Cannot store a decimal number to a type int variable: %s\n",words[j]);
					return ERROR;
				}
				struct_arr[k].x=(int)x;		/* Cast double to int */
			}
			else if ( x == (int)x ){		/* It is int and we have to create a new variable */
				strcpy(struct_arr[size].name,words[i]);
				struct_arr[size].x=(int)x;
				size++;
				if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
					printf("Could not allocate memory \n");
					return ERROR;
				}
			}
			else{				/* It is double and we have to create a new variable */
				strcpy(ipl_double[double_size].name,words[i]);
				ipl_double[double_size].x=x;
				double_size++;
				if ( ( ipl_double =(struct double_num *) realloc ( ipl_double,(double_size + 1 ) * sizeof ( struct double_num ) ) ) == NULL ){
					printf("Could not allocate memory \n");
					return ERROR;
				}
			}
			if ( words[++i] != NULL ){
				printf("Expected new line\n");
				return ERROR;
			}
		}
	}
	else if (strcmp(words[i],"write") == 0 || strcmp(words[i],"writeln") == 0){
		if (write_fun(words,words[i],i+1) == ERROR ){
			return ERROR;
		}		
	}
	else if (check_var(words[i]) == 1){		/* We will insert a value at this variable */
		if ( find_array (words[i]) != array_size ){
			printf("This name is already used to an array %s\n",words[i]);
			return ERROR;
		}
		if (strcmp(words[i+1],"=") == 0){
			temp=expression_calculate(words,i+2);
			if ( temp.ok == 0){
				return ERROR;
			}
			if (temp.its_double) {
				if ( find_var(words[i]) != size ){
					printf("Variable with this name is type int: %s\n",words[i]);
					return ERROR;
				}
				if ( ( k = find_double(words[i]) ) != double_size) {
					ipl_double[k].x=temp.x;
				}
				else if ( find_array(words[i]) != array_size ){
					printf("There is already an array with this name: %s\n",words[i]);
					return ERROR;
				}
				else{
					strcpy(ipl_double[double_size].name,words[i]);
					ipl_double[double_size].x=temp.x;
					double_size++;
					if ( ( ipl_double =(struct double_num *) realloc ( ipl_double,(double_size + 1 ) * sizeof ( struct double_num ) ) ) == NULL ){
						printf("Could not allocate memory \n");
						return ERROR;
					}
				}
			}
			else{
				if ( ( k= find_double(words[i]) )!= double_size ){
					ipl_double[k].x = temp.x;
				}
				else if ( find_array(words[i]) != array_size ){
					printf("There is already an array with this name: %s\n",words[i]);
					return ERROR;
				}
				else if ( ( k=find_var(words[i]) ) != size){
					struct_arr[k].x=(int)temp.x;
				}
				else{
					strcpy(struct_arr[size].name,words[i]);
					struct_arr[size].x=(int)temp.x;
					size++;
					if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
						printf("Could not allocate memory \n");
						return ERROR;
					}
				}
			}
		}
		else{
			printf("Expected '=' after %s\n",words[i]);
			return ERROR;
		}
	}
	else if (check_var(words[i]) == ITS_ARRAY){
		if (strcmp(words[i+1],"=") == 0){
			if ( ( k=find_array(words[i]) ) != array_size){			/* Found an array with such name */
				temp=expression_calculate(words,i+2);
				if (temp.ok == 0){
					return ERROR;
				}
				if ( temp.its_double ){
					printf("Epxected integer\n");
				}
				if ( commands_array(words[i],"insert",temp.x,NULL) == ERROR ){		/* Insert temp.x to the element that words[i] indicates */
					return ERROR;
				}
			}
			else{
				printf("There is no array with such name\n");
				return ERROR;
			}
		}
	}
	else if (strcmp(words[i],"while") == 0){
		condition=check_cond(words);
		if ( condition == ERROR ){
			return ERROR;
		}
		else if (condition == 0){		/* The condition is not to true, so we have to pass the loop */
			return NO_WHILE;
		}
		else {
			return YES_WHILE;		/* Get inside the loop */
		}
	}
	else if (strcmp(words[i],"random") == 0 ){
		i++;
		if (check_var(words[i]) == 1){
			k=find_var(words[i]);
			if (k != size){
				struct_arr[k].x=rand();
			}
			else if ( ( k = find_double(words[i]) ) != double_size ){
				ipl_double[k].x = (double)rand();
			}
			else{
				strcpy(struct_arr[size].name,words[i]);
				struct_arr[size].x=rand();
				size++;
				if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
					printf("Could not allocate memory \n");
					return ERROR;
				}
			}
		}
		else if (check_var(words[i]) == ITS_ARRAY){
			if ( commands_array(words[i],"random",666,NULL)  == ERROR){
				return ERROR;
			}
		}
	}
	else if(strcmp(words[i],"if") == 0){
		condition=check_cond(words);
		if ( condition == ERROR ){
			return ERROR;
		}
		else if (condition == 0){		/* Pass if */
			return NO_IF;
		}
		else{
			return YES_IF;
		}
	}
	else if ( strcmp(words[i],"else") == 0){
		if ( words[++i] != NULL ){
			printf("Expected new line\n");
			return ERROR;
		}
		return YES_ELSE;			/* Return a value that indicates we found 'else' and other functions are responsible to deal with it */ 
	}
	else if (strcmp(words[i],"argument") == 0 ){
		i++;
		if ( strcmp(words[i],"size") == 0 ) {	/* Return a value that indicates that we found 'argument size' */
			return SIZE_ARGUMENT;
		}
		else{
			return ARGUMENT;		/* Retun a value that indicates that we found 'argument' */
		}
	}
	else if ( strcmp(words[i],"break") == 0 ){
		break_count=1;				/* By default break means that we break only one loop */
		if ( words[++i] != NULL ){
			temp=expression_calculate(words,++i);
			if ( !temp.ok){
				return ERROR;
			}
			if ( temp.its_double || temp.x <= 0 ){
				printf("Expected positive integer after continue");
				return ERROR;
			}
			break_count = (int)temp.x;	/* Break_count indicates the number of loops that have to be broken */
		}
		return YES_BREAK;
	}
	else if (strcmp(words[i],"continue") == 0 ){
		continue_count=1;
		if ( words[++i] != NULL ){
			temp=expression_calculate(words,i);
			if ( !temp.ok){
				return ERROR;
			}
			if ( temp.its_double || temp.x <= 0 ){
				printf("Expected positive integer after continue");
				return ERROR;
			}
			continue_count = (int)temp.x;
		}
		return YES_CONTINUE;
	}
	else if (strcmp(words[i],"new") == 0 ){
		if (find_var(words[i+1]) != size ){
			printf("This name is already used for an int variable %s\n",words[i+1]);
			return ERROR;
		}
		if (find_double(words[i+1]) != double_size ){
			printf("This name is already used for a double variable %s\n",words[i+1]);
			return ERROR;
		}
		if ( create_array(words,++i) == ERROR ){
			return ERROR;
		}
		if ( words [++i] != NULL){
			printf("Expected new line\n");
			return ERROR;
		}
	}
	else if (strcmp(words[i],"free") == 0){
		if ( free_array(words,++i) == ERROR ){
			return ERROR;
		}
		if ( words [++i] != NULL){
			printf("Expected new line\n");
			return ERROR;
		}
	}
	else if (strcmp(words[i],"size") == 0){		/* Need to get the size of a specifin array */
		i++;
		if ( ( k = find_array(words[i]) == array_size)){		
			printf("There is no array with such name: %s\n",words[i]);
		}
		j=all_arrays[k]->dim;			/* We found the dimension of the array */
		i++;
		if ( check_var(words[i]) == ERROR){
			return ERROR;
		}						/* We are going to find the variable that is going to store the size */
		else if ( check_var(words[i]) == ITS_ARRAY){
			commands_array(words[i],"insert",j,NULL);
		}
		else{
			if ( (k=find_var(words[i]) ) !=size ){
				struct_arr[k].x=j;
			}
			else if ( ( k = find_double(words[i]) ) != double_size ){
				ipl_double[k].x=(double)j;
			}
			else{
				strcpy(struct_arr[size].name,words[i]);
				struct_arr[size].x=j;
				size++;
				if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
					printf("Could not allocate memory \n");
					return ERROR;
				}
			}
		}
		if ( words[++i] != NULL ){
			printf("Expected new line\n");
			return ERROR;
		}
	}
	else if (strcmp(words[i],"seed") == 0){		/* Seed initialization */
		temp=expression_calculate(words,++i);
		if (!temp.ok){
			return ERROR;
		}
		if (temp.its_double){
			printf("Expected integer after seed\n");
			return ERROR;
		}
		srand( (unsigned int)temp.x);
	}
	else if (strcmp(words[i],"for") == 0){			/* Check if there is any syntax mistake, in case there is not return a value that indicates we are in a for loop */
		if ( strcmp(words[i+2],"from") != 0 ){
			printf("Expected 'from' in a declaration of a for loop\n");
			return ERROR;
		}
		i+=2;
		while ( words[i] != NULL && strcmp(words[i],"to") !=0 ){
			i++;
		}
		if ( words[i] == NULL ) {
			printf("Expected 'to' in a declaration of a for loop\n");
			return ERROR;
		}
		i++;
		while (words[i] != NULL && strcmp(words[i],"step") != 0 ){
			i++;
		}
		if ( words[i] == NULL ){
			printf("Expected 'step' in a declaration of a for loop for loop\n");
			return ERROR;
		}
		return YES_FOR;
	}
	else{
		fprintf(stderr,"Invalid command: %s\n",words[i]);
		return ERROR;
	}
	return 0;
}

