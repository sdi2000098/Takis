#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"


char ** word_seperation ( char ch[1000] )		/* This is a function that seperates the words of a line */
{	

	/* Gets as argument the line and returns a pointer to an array of strings */
	/* Each line of the array is a different word */
	extern int size;	
	int i,j;
	int number_of_parenthesis = 0 ;		/* This varialbe is going to be used to check if the number ot left parenthesis is equal to the number of right parenthesis */
	char **array;
	array=malloc(sizeof( char * ));			/* Allocate memory for the array that is going to be returned */
	if ( array == NULL ){
		fprintf(stderr,"Could not allocate memory \n");
		return NULL;
	}
	array[0]=malloc(100*sizeof(char));
	if ( array[0] == NULL ){
            	fprintf(stderr,"Could not allocate memory \n");	
           	return NULL;
       	}
	if ( ch[0] == '\n' || ch[0] == '#'){			/* The line is either full of comments or empty */
		array = realloc(array,2*sizeof(char*));
		if ( array == NULL ){
			fprintf(stderr,"Could not allocate memory \n");
			return NULL;
		}
		array[1] = malloc(100*sizeof(char));
		if ( array[1] == NULL ){
            		fprintf(stderr,"Could not allocate memory \n");	
           		return NULL;
       		}
		array[1][0] = '#';
		array[1][1] = '\0';
		array[0]=NULL;					/* In this way we indicate we have to pass this line */
		return array;
	}
	int k=0;
	while ( ch[k] == ' ' || ch[k] == '\t' ){		/* Pass spaces and tabs in the beginning */
		k++;
		if (ch[k] == '\n' || ch[k] == '#'){		/* In case the line is full of tabs, spaces, comments  and new line */
			array[0]=NULL;
			array = realloc(array,2*sizeof(char*));
			if ( array == NULL ){
				fprintf(stderr,"Could not allocate memory \n");
				return NULL;
			}
			array[1] = malloc(100*sizeof(char));
			if ( array[1] == NULL ){
            			fprintf(stderr,"Could not allocate memory \n");	
           			return NULL;
       			}
			array[1][0] = '#';
			array[1][1] = '\0';
			return array;
		}
	}
	k=0;
	i=0;
	j=0;
	while ( ch[k]=='\t'){				/* Find the tabs at the beggining and store them all at the first line of the array */
		array[0][j++]='\t';
		k++;
		i=1;
	}
	array[0][j] = '\0';
	if ( i == 1){
		array = realloc(array,(i+1)*sizeof(char*));
		if ( array == NULL ){
			fprintf(stderr,"Could not allocate memory \n");
			return NULL;
		}
		array[i] = malloc(100*sizeof(char));
	}
	if ( array[i] == NULL ){
        	fprintf(stderr,"Could not allocate memory \n");	
        	return NULL;
       	}
	while ( ch[k] != '\0' ){
		j=0;
		if ( ch[k] == '"' ){
			array[i][j++]=ch[k++];
			while ( ch[k] != '"' && ch[k] != '\0'){
				array[i][j++]=ch[k++];
			}
			if ( ch[k] == '\0' ){
				fprintf(stderr,"Missing terminating ""\n");
				return NULL;
			}
			array[i][j++]=ch[k++];
			array[i][j] = '\0';
		}
            	while ( ch[k] != ' ' && ch[k] != '\0' && ch[k]!= '\t' && ch[k] != '#' && ch[k] != '\n' && ch[k]!='"'){		/* Numbers letters etc are all stored in an array line until we have a reason to stop such as spaces or tabs */
               		array[i][j++]=ch[k++];
            	}	
		array[i][j] = '\0';
		if ( array[i][0] == '('){
			number_of_parenthesis++;
		}
		else if ( array[i][0] == ')' ){
			number_of_parenthesis--;
		}
		i++;				/* Go for the next word */
		array = realloc(array,(i+1)*sizeof(char*));
		if ( array == NULL ){
			fprintf(stderr,"Could not allocate memory \n");
			return NULL;
		}
		array[i] = malloc(100*sizeof(char));
		if ( array[i] == NULL ){
            		fprintf(stderr,"Could not allocate memory \n");	
           		return NULL;
       		}
		while ( ch[k] == ' ' || ch[k] == '\t'){
			k++;			/* Go for the next character in the text line */
		}
		if ( ch[k] == '\0' || ch[k] == '\n' || ch[k] == '#' ){		/* All characters are read */
				break;
		}
	}
	array[i]=NULL;			/* Indicates the end of line */
	if ( number_of_parenthesis ){
		fprintf(stderr,"Invalid number of parenthesis: ");
		i=0;
		while ( array[i] != NULL ){
			fprintf(stderr,"%s ",array[i++]);
		}
		putchar('\n');
		return NULL;
	}
	return array;
}
