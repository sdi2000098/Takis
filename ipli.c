#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

int break_count=0,continue_count=0,size = 0,double_size=0;
struct var * struct_arr ;
struct double_num * ipl_double ;

int main ( int argc, char *argv[] )
{
	extern int break_count,continue_count,array_size;
	extern struct ipl_arrays ** all_arrays;
	char **words,**text;
	int x,start,end,count_tabs,i,j,last_if=0,k,m,check,lines=0;
	FILE *fp=NULL;
	if ( (text=malloc(sizeof(char *))) == NULL){				/* An array of strings used to store each line of the IPL file */
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	if ( (text[0]=malloc(1000*sizeof(char ))) == NULL){			/* Assuming that each line contains maximum 1000 chars */
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	if ( argc<2 ){
		fprintf(stderr,"Not enough arguments \n");
		return ERROR;
	}
	if ( strcmp ( "-v",argv[1] ) ==  0 ){
		if ( argc < 3 ){
			fprintf(stderr,"Not enough arguments \n");
			return ERROR;
		}
		fp  = fopen (argv[2],"r");
	}
	else{
		fp = fopen (argv[1],"r");
	}
	if ( fp == NULL ){
		fprintf(stderr,"The file could not be opened \n");
		return ERROR;
	}
	if ( ( struct_arr =(  struct var * )(malloc ((size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){			/* Allocating memory to store the first variable */
		fprintf(stderr,"Could not allocate memory \n");
		return ERROR;
	}
	if ( ( ipl_double = ( struct double_num *) ( malloc ((double_size + 1) * sizeof (struct double_num ) ) ) ) == NULL ){
		fprintf(stderr,"Could not allocate memory \n");
		return ERROR;
	}
	while  (feof (fp) == 0 ){
		fgets(text[lines],1000,fp);	
		lines++;
		if ( ( text=realloc(text,(lines+1)*sizeof(char*)) ) == NULL){							/* Allocating memory to stor the next lin */
			fprintf(stderr,"Could not allocate memory\n");
			return ERROR;
		}
		if ( (text[lines] = malloc(1000*sizeof(char))) == NULL){
			fprintf(stderr,"Could not allocate memory\n");
			return ERROR;
		}
	}
	for (i=0;i<lines;i++){						/* For each line of the ipl program */
		if ( (words=word_seperation(text[i]) ) == NULL){
			return ERROR;
		}
		if ( strcmp ( argv[1],"-v" ) == 0 && words[0]!= NULL){
			printf("%d:	",i+1);
		}
		if ( (x=commands(words,0,argv[1])) == ERROR){									/* The function commands determines the operation and return different values in case the main function nead to treat this commands in a different way */
			return ERROR;
		}
		else if ( x== 0 ){
			continue;
		}
		if( x == YES_WHILE || x==NO_WHILE || x == YES_IF || x==NO_IF || x==YES_ELSE || x==YES_FOR){					/* Finding the limits of the loop  */
			start=i;												/* We will store the starting line in order to be able to check again the condition in while loops */
			end = find_end(text,&i,0,lines);
			if (x==YES_WHILE){
				check=while_fun(start,end,text,1,argv[1],argc,argv);
				if ( check == ERROR){
					return ERROR;
				}
				else if ( check == YES_BREAK || check == YES_CONTINUE ){
					printf("Cannot be used outside while or for loop \n");
				}
			}
			else if (x==YES_IF){
				check = if_fun(start,end,text,1,argv[1],argc,argv);
				if (  check == ERROR ){
					return ERROR;
				}
				else if ( check == YES_BREAK || check == YES_CONTINUE ){
					printf("Cannot be used outside while or for loop \n");
				}
				last_if=1;
			}
			else if (x==NO_IF){
				last_if=0;
			}
			else if (x==YES_ELSE){
				if (last_if==0){
					check = if_fun(start,end,text,1,argv[1],argc,argv);
					if ( check == ERROR ){
						return ERROR;
					}
					else if ( check == YES_BREAK || check == YES_CONTINUE ){
						printf("Cannot be used outside while or for loop \n");
					}
				}
			}
			else if ( x == YES_FOR ){
				check=for_fun(start,end,text,1,argv[1],argc,argv);
				if ( check == ERROR ){
					return ERROR;
				}
				if ( check == YES_BREAK || check ==  YES_CONTINUE ){
					printf("Cannot be used outside while or for loop \n");
					return YES_BREAK;
				}
			}
			i=end;
		}
		else if ( x == SIZE_ARGUMENT || x == ARGUMENT ){
			if ( arguments(x,words,argc,argv) == ERROR ){
				return ERROR;
			}
		}
		else if ( x == YES_BREAK && break_count != 0 ){
			fprintf(stderr,"Break cannot be used outside while or for loop \n");
			return ERROR;	
		}
		else if ( x== YES_CONTINUE && continue_count !=0 ){
			fprintf(stderr,"Contnue cannot be used outside while or for loop\n");
			return ERROR;
		}
		j = 0;
	}
	for ( j=0;j<array_size;j++){
		free(all_arrays[j]->array);
		free(all_arrays[j]);		
	}
	free(all_arrays);
	free(struct_arr);
	free(ipl_double);
	free(text);
	if ( ferror(fp) ){
		fprintf(stderr,"Read error\n");
		fclose(fp);
		return ERROR;
	}
	fclose(fp);
	return 0;
}

