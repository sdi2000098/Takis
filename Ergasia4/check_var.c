#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

int check_var (char  words[100] )			/* Function that gets a possible variable name end checks if it is valid */
{   
	extern int array_size;
	char keywords[][20]={"read","write","writeln","while","if","else","random","break","continue","new","free","size","argument","break","continue","free","new","size","seed","for","from","to","step"};
	if (  (words[0] < 'A' || words[0] > 'Z' ) && ( words[0] <'a' || words[0] >'z' )){
		return ERROR;
	}
	int i=1;
	while ( words[i] != '\0' ){			/* Check every lettter of the possible name */
		if ( (words[i] < 'A' || words[i] > 'Z' ) && ( words[i] <'a' || words[i] >'z' ) && words[i] != '_' && words[i] != '[' && words[i] !=']' &&( words[i] >'9' || words[i] < '0' )){
			return ERROR;
		}
		else{
			i++;
		}
	}
	if ( find_array ( words) != array_size){			/* There is already an array with such name, function returns a value that indicates that the name is used for an array */
		return ITS_ARRAY;
	}
	for ( i=0;i<MAXWORDS;i++ ){
		if ( strcmp ( words,keywords[i] ) == 0 ){		/* The name is a keyword, cant be used as variable name */
			return ERROR;
		}
	}
	return 1;
}


int find_var (char words[100])			/* Function that gets a name as argument and check whether there is already an existing int variable with such name */
{   						/* Returns an index to access the variable in case it exists or an index to create a new variable in case it does not exist */
	int k;
	extern struct var * struct_arr;
	extern int size;
	for (k=0;k<size;k++){
		if (strcmp(words,struct_arr[k].name) == 0){
			return k;
		}
	}
	return size;
}


int find_array (char ch[100])			/* Function that get a name as argument and check whether there is already an existing array with such name */
{						/* Returns an index to access the array in case it exists or an index to create a new array in case it does not exist */
	extern struct ipl_arrays ** all_arrays;
	int i,k =0;
	char words[100];
	while (ch[k] != '[' && ch[k]!='\0'){	/* Seperate the name of the array */
		words[k]=ch[k];
		k++;
	}
	words[k]='\0';
	extern int array_size;
	for ( i = 0; i < array_size; i++){
		if (strcmp( words,all_arrays[i]->name) == 0){
			return i;
		}
	}
	return array_size;
}

int find_double ( char ch[100] )		/* Function that get a name as argument and check whether there is already an existing double variable with such name */
{						/* Returns an index to access the array in case it exists or an index to create a new array in case it does not exist */
	extern struct double_num * ipl_double ;
	extern int double_size;
	int k;
	for ( k = 0;k<double_size;k++){
		if ( strcmp(ch,ipl_double[k].name) == 0 ){
			return k;
		}
	}
	return double_size;
}