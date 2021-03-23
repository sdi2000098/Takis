#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

/* A function that is responsible for dealing with arguments */


int arguments ( int x, char ** words, int argc, char *argv[])
{
	int j,k,m;
	extern struct double_num * ipl_double ;
	extern struct var * struct_arr;
	extern int size,array_size,double_size;
	extern struct double_num * ipl_double;
	int check_v;
	if (strcmp("-v",argv[1]) == 0){
        	check_v=1;				/* check_v is used in case we are in mode '-v', if so the arguments of the ipl program are thos after '-v' */
	}
	else{						/* In fact check_v indicates wheter we have to pass one more arguments or nor */
        	check_v=0;
	}
	if ( x== SIZE_ARGUMENT ){			/* Function will return the number of arguments */
		if (words[0][0]=='\t'){			/* Pass spaces */
			j=1;
		}
		else{
			j=0;
		}
		j+=2;					/* Go for the variable that is going to store the arguments size */
		if ( check_var(words[j]) == ERROR ){
			fprintf (stderr,"Invalid name for variable \n");
			return ERROR;
		}
		if ( ( k = find_double ( words[j] ) )!= double_size ){
			ipl_double[k].x = (double) (argc - 2 - check_v);	/* The original argc without the name of the c program, without the name of ipl program and without the possible '-v' */
		}
		else if ( (k = find_var(words[j]) ) != size ){			/* There is already a variable with this name */
            		struct_arr[k].x=argc-2-check_v;
		}
		else if ( check_var(words[j]) == ITS_ARRAY){		/* Insert the value to an element of an array */
            		commands_array(words[j],"insert",argc-2-check_v,NULL);
		}
		else {							/* Create a new variable */
			strcpy(struct_arr[size].name,words[j]);
            		struct_arr[size].x=argc-2-check_v;
			size++;
			if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
			/* Allocating memory for the new variable */
				fprintf(stderr,"Could not allocate memory \n");
				return ERROR;
			}
		}
	}
	else if ( x == ARGUMENT ){			/* In this case, the function will return the value a specific argument */
		if (words[0][0]=='\t'){			/* Pass tabs */
			j=1;
		}
		else{
			j=0;
		}
		j++;					/* Going to find which argument is going to be used */
		if ( words[j] == NULL ){
			fprintf(stderr,"Not enough arguments\n");
			return ERROR;
		}
		if ( find_double ( words[j] ) != double_size ){
			fprintf(stderr,"Expected integer and there is already a double type variable with this name %s\n",words[j]);
			return ERROR;
		}
		if ( words[j][0] == '0' && atoi(words[j]) == 0){
			fprintf(stderr,"Expected integer bigger than 0\n");
			return ERROR;
		}
		else if ( atoi(words[j]) != 0 ){
			k=atoi(words[j]);			/* k variable indicates the argument */
		}
		else{				/* It has to be variable or element of an array */
			m=find_var(words[j]);
			if ( check_var(words[j]) == 1 && m == size ){
			/* Valid name for variable but not an existing one, since the initialized value for a new variable is zero, the input is dismissed either way*/
				fprintf(stderr,"Invalid input:%s\n",words[j]);
				return ERROR;
			}
			else if ( check_var(words[j]) == ITS_ARRAY){
				k=commands_array(words[j],"calculate",666,NULL);		/* Calculate the value of this element */
			}
			else if (m != size){						/* Get the value of the variable */
				k=struct_arr[m].x;
			}
		}
		if ( k < 1 ){							/* Only integers bigger than 0 are acceptable */
			printf("Expected integer bigger than 0\n");
			return ERROR;
		}
		else{
			j++;							/* Find the variable to store data */
	                if (argv[k+1+check_v] == NULL ) {			/* +1 in order to pass the name of c and ipl program, + check_v in order to pass the possible '-v' arguments */
                        	fprintf(stderr,"Not enough arguments\n");
                        	return ERROR;
                    	}
			if ( words[j] == NULL ){				/* There is no variable to store the value of argument */
				fprintf(stderr,"Not enough arguments");
				return ERROR;
			}
			if ( check_var(words[j]) == ERROR ){
				fprintf (stderr,"Invalid name for variable %s\n",words[j]);
				return ERROR;
			}
			else if ( check_var(words[j]) == ITS_ARRAY){
                		if ( argv[k+1+check_v][0] == '0' && atoi(argv[k+1+check_v]) == 0){
                    			commands_array(words[j],"insert",0,NULL);
               	 		}
                		else if ( atoi(argv[k+1+check_v]) != 0 ){
                    			commands_array(words[j],"insert",atoi(argv[k+1+check_v]),NULL);
                		}
                		else{
                   		 	fprintf(stderr,"Expected integer as input\n");
                    			return ERROR;
                		}		
			}
			else{				/* It is variable */
				m=find_var(words[j]);
				if ( m != size ){	/* Already existing variable */
                    			if ( argv[k+1+check_v][0] == '0' && atoi(argv[k+1+check_v]) == 0){
                       			 	struct_arr[m].x=0;
                    			}
                    			else if ( atoi(argv[k+1+check_v]) != 0 ){
                        			struct_arr[m].x=atoi(argv[k+1+check_v]);
                    			}
                   		 	else{
                        			fprintf(stderr,"Expected integer as input\n");
                        			return ERROR;
                    			}
				}
				else if ( ( m=find_double(words[j]) )!= double_size){		/* Already existing double variable */
                    			if ( argv[k+1+check_v][0] == '0' && atof(argv[k+1+check_v]) == 0){
                        			ipl_double[m].x=0;
                    			}
                    			else if ( atof(argv[k+1+check_v]) != 0 ){
                        			ipl_double[m].x=atof(argv[k+1+check_v]);
                    			}
                    			else{
                        			fprintf(stderr,"Expected double as input\n");
                        			return ERROR;
                    			}
                		}
                		else{		/* Create a variable */
                    			if ( (double)atoi(argv[k+1+check_v]) == atof(argv[k+1+check_v]) ){	/* It is an integer */
                        			strcpy(struct_arr[size].name,words[j]);
                        			if ( argv[k+1+check_v][0] == '0' && atoi(argv[k+1+check_v]) == 0){
                            				struct_arr[size].x=0;
                        			}
						else if ( atoi(argv[k+1+check_v]) != 0 ){
							struct_arr[size].x=atoi(argv[k+1+check_v]);
						}
                        			else{
                            				fprintf(stderr,"Expected integer as input\n");
                            				return ERROR;
                        			}
						size++;
                        			if ( ( struct_arr = (struct var *)realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) ) ) == NULL ){
                            				fprintf(stderr,"Could not allocate memory \n");
                            				return ERROR;
                        			}
                    			}
                    			else{									/* It is a double */
                        			strcpy(ipl_double[double_size].name,words[j]);
                        			if ( argv[k+1+check_v][0] == '0' && atof(argv[k+1+check_v]) == 0){
                            				ipl_double[double_size].x=0;
                       		 		}
                        			else if ( atof(argv[k+1+check_v]) != 0 ){
                            				ipl_double[double_size].x=atof(argv[k+1+check_v]);
                        			}
                        			else{
                            				fprintf(stderr,"Expected double as input\n");
                            				return ERROR;
                        			}
						double_size++;
                        			if ( ( ipl_double =(struct double_num *) realloc ( ipl_double,(double_size + 1 ) * sizeof ( struct double_num ) ) ) == NULL ){
                            				fprintf(stderr,"Could not allocate memory \n");
                            				return ERROR;
                        			}
                    			}
                		}
			}
		}
	}
	if ( words[++j] != NULL ){
		fprintf(stderr,"Expected new line\n");
		return ERROR;
	}
	return 0;
}
