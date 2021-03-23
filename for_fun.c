#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

int find_address (char * name, double ** point_double, int ** point_int)		/* A function that is used to find the adress of a spcific variable. There are 2 pointers, if the variable is type int the int pointer is used to store the addres, if the variable is type double the double pointer is used*/
{	/* Getting as argument the name of the variable and returns 0 in case everything is fine */
	extern int  double_size,size;
	extern struct double_num * ipl_double;
	extern struct var * struct_arr;
	int k;
	if ( check_var (name) == ERROR ){
		fprintf(stderr,"Invalid variable name: %s \n",name);
		return ERROR;
	}
	if ( ( k = find_double(name) ) !=  double_size ){
		*point_double = &(ipl_double[k].x);		/* Store the addres of the double variable */
		*point_int = NULL;
		return 0;
	}
	if ( ( k = check_var(name) ) == ITS_ARRAY ){
		if ( commands_array(name,"address",666,point_int ) == ERROR){		/* Store the addres of the element of the array */
			return ERROR;
		}
		*point_double = NULL;
		return 0;
	}
	if ( ( k = find_var(name) ) != size ){
		*point_int = &(struct_arr[k].x);			/* Store the addres of the int variable */
		*point_double = NULL;
		return 0;
	}
	struct_arr[size].x =0;			/* Create a new variable and store its addres*/
	strcpy(struct_arr[size].name,name);
	size++;
	if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
		fprintf(stderr,"Could not allocate memory \n");
		return ERROR;
	}
	*point_int = &(struct_arr[size-1].x);
	*point_double = NULL;
	return 0;
}

int for_fun(int start, int end,char ** text,int tabs,char * print_line, int argc,char * argv[])		/* A function to deal with for loop*/
{	/* Gets asa arguments the limits, the lines of the ipl program, the amount of tabs used in the loop, print_line stands for  mode '-v'. There are also argc and argv in case they are needed */ 
	int i,x,new_start,new_end,count_tabs,last_if=0,check,*point_int=NULL,expression,j;
	char ** words,var_name[100];
	extern int break_count,continue_count;
	double *point_double=NULL,cond,step;
	struct valid temp;
	if ( (words=word_seperation(text[start]) ) == NULL){		/* Seperate words  of the first line which contains the statement of for loop */
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	i = 0;
	if ( words[i][0] == '\t' ){		/* Pass tabs */
		i++;
	}
	/* Commands function has already checked for syntax error so we only have to find variable, step and the condition */
	if ( find_address(words[++i],&point_double,&point_int) == ERROR ){		/* Find the addres of the variable used in for loop */
		return ERROR;
	}
	strcpy(var_name,words[i]);
	i += 2;				
	temp = expression_calculate(words,i);			/* Find the value that our variable is going to get */
	if ( !temp.ok ){
		return ERROR;
	}
	if ( point_int != NULL ){
		*point_int=(int)temp.x;
	}
	else{
		*point_double = temp.x;
	}
	while (strcmp(words[i],"to") != 0 ){			/* Pass all words that were used to the expression calculation */
		i++;
	}
	i++;
	temp = expression_calculate(words,i);			/* Find the limit which is going to form the condition */
	if ( !temp.ok ){
		return ERROR;
	}
	cond = temp.x;
	while (strcmp(words[i],"step") != 0 ){			/* Pass all words that were used to the expression calculation */
		i++;
	}
	i++;
	temp = expression_calculate(words,i);			/* Find the step */
	if ( !temp.ok ){
		return ERROR;
	}
	step = temp.x;
	if ( step >= 0 ){					/* Since the step is bigger than 0 the operand will be '<=' */
		if ( point_int != NULL ){
			expression = (*point_int <= cond );
		}
		else{
			expression = ( *point_double <= cond );
		}
	}		
	else{							/* Since the step is smaller than 0 the operand will be '>=' */
		if ( point_int != NULL ){
			expression = (*point_int >= cond );
		}
		else{
			expression = ( *point_double >= cond );
		}
	}
	i = 0;
	while(words[i] != NULL ){
		free(words[i++]);
	}
	free(words);
	while (expression){
		for (i=start+1;i<=end;i++){			/* For each line of the loop */
			if ( (words=word_seperation(text[i]) ) == NULL){		/* Seperate words */
				fprintf(stderr,"Could not allocate memory\n");
				return ERROR;
			}
			if ( strcmp(print_line,"-v" ) == 0 ){
				if ( i != end ){
					printf("%d:	",i);
				}
				else{
					printf("%d:	",start);
				}
			}
			x=commands(words,tabs,print_line);	/* Deal with the commands in the line */
			if ( x == ERROR){
				return ERROR;
			}
			else if ( x== 0 ){			/* Line full of comments or empty one */
				continue;
			}
			else if ( x == YES_BREAK ){		/* We have to break the loop */
				break_count--;
				if ( break_count == 0 ){	/* We only need to break this for loop */
					return 0;
				}
				else{
					return YES_BREAK;	/* We have break not only this for loops but more  nested loops */
				}
			}
			else if ( x==YES_CONTINUE ){
				continue_count--;
				if ( continue_count == 0 ){	/* We have to pass one circle of the for loop */
					break;
				}
				else{				/* We have to break this loop and use continue to another nested loop */
					return YES_CONTINUE;
				}
			}
			else if ( x == YES_WHILE || x==NO_WHILE || x == YES_IF || x==NO_IF || x==YES_ELSE || x == YES_FOR){		/* Prepare for the nested loop */
				new_start=i;					/* New start and new end indicate the limits of the new loop */
				new_end = find_end(text,&i,tabs,end);
				if (x==YES_WHILE){
					check=while_fun(new_start,new_end,text,tabs+1,print_line,argc,argv);		/* Start a while loop with the new limits and one extra tab */
					if ( check == ERROR ){
							return ERROR;			/* While function may return the signal of a break commands or a continue command */
					}
					else if ( check == YES_BREAK ){
						break_count--;
						if ( break_count == 0 ){
							return 0;
						}
						else {
							return YES_BREAK;
						}
					}
					else if ( check == YES_CONTINUE ){
						continue_count--;
						if ( continue_count == 0 ){
							break;
						}
						else{
							return YES_CONTINUE;
						}
					}
				}
				else if (x==YES_IF){
					check = if_fun(new_start,new_end,text,tabs+1,print_line,argc,argv) ;
					if ( check == ERROR ){
						return ERROR;
					}
					else if ( check == YES_BREAK ){
						break_count--;
						if ( break_count == 0 ){
							return 0;
						}
						else {
							return YES_BREAK;
						}
					}
					else if ( check == YES_CONTINUE ){
						continue_count--;
						if ( continue_count == 0 ){
							break;
						}
						else{
							return YES_CONTINUE;
						}
					}
					last_if=1;		/* Last_if is used in case we find 'else' command later. Since the condition of if is true, last_if gets the value 1 */
				}
				else if (x==NO_IF){
					last_if=0;		/* Condition false, lat_if gets 0 */
				}
				else if (x==YES_ELSE){
					if (last_if==0){	/* Will get to this loop only if the condition of previous if was false */
						check=if_fun(new_start,new_end,text,tabs+1,print_line,argc,argv);
						if ( check == ERROR ){
							return ERROR;
						}
						else if ( check == YES_BREAK ){
							break_count--;
							if ( break_count == 0 ){
								return 0;
							}
							else {
								return YES_BREAK;
							}
						}
						else if ( check == YES_CONTINUE ){
							continue_count--;
							if ( continue_count == 0 ){
								break;
							}
							else{
								return YES_CONTINUE;
							}
						}
					}
				}
				else if ( x == YES_FOR ){	/* Recursive nested for loop with new limits */  
					check=for_fun(new_start,new_end,text,tabs+1,print_line,argc,argv);
					if ( check == ERROR ){
						return ERROR;
					}
					else if ( check == YES_BREAK ){
						break_count--;
						if ( break_count == 0 ){
							return 0;
						}
						else {
							return YES_BREAK;
						}
					}
					else if ( check == YES_CONTINUE ){
						continue_count--;
						if ( continue_count == 0 ){
							break;
						}
						else{
							return YES_CONTINUE;
						}
					}
				}
				i=new_end;
			}
			else if ( x == SIZE_ARGUMENT || x == ARGUMENT ){		/* Use the arguments function */
				if ( arguments(x,words,argc,argv) == ERROR ){
					return ERROR;
				}
			}
			j = 0;
			while (words[j] != NULL ){
				free(words[j++]);
			}
			free(words);
		}
		if ( find_address(var_name,&point_double,&point_int) == ERROR ){	/* Since we dynamically allocate memory for new variables, the variable used in for loop may have changed addres. If so we are going to find the new addres */
			return ERROR;
		}
		if ( point_int != NULL ){				/* Increase the variable by step and check the condition */
			*(point_int) = *(point_int) + (int)step ;
			if ( step >= 0 ){
				expression = ( *(point_int) <= cond);
			}
			else{
				expression = ( *(point_int) >= cond );
			}
		}
		else{
			*(point_double) = *(point_double) + step;
			if ( step >= 0 ){
				expression = ( *(point_double) <= cond);
			}
			else{
				expression = ( *(point_double) >= cond );
			}
		}
		if ( strcmp(print_line,"-v") == 0 ){
			printf("%d: 	%s",start,text[start]);
		}
	}
	return 0;
}








