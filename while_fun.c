#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"


int while_fun (int start,int end,char **text, int tabs,char * print_line,int argc,char * argv[])		/* Function to deal with while loops */
{
	int i,x,new_start,new_end,count_tabs,j,last_if=0,check;
	char **words;
	extern int break_count,continue_count;
	do{
		for (i=start+1;i<=end;i++){	/* For each line */
			if ( (words=word_seperation(text[i]) ) == NULL){
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
			x=commands(words,tabs,print_line);
			if ( x == ERROR){
				return ERROR;
			}
			else if ( x== 0 ){
				continue;
			}
			else if ( x == YES_BREAK ){		
				break_count--;
				if ( break_count == 0 ){
					return 0;		/* Break this loop only */
				}
				else{
					return YES_BREAK;	/* Break this loop and return a signal that more loops are to be broken */
				}
			}
			else if ( x==YES_CONTINUE ){
				continue_count--;
				if ( continue_count == 0 ){	/* Pass one circle of while loop */
					break;
				}
				else{
					return YES_CONTINUE;	/* Break the loop and return a signal to continue to more loops */
				}
			}
			else if ( x == YES_WHILE || x==NO_WHILE || x == YES_IF || x==NO_IF || x==YES_ELSE || x == YES_FOR){
				new_start=i;
				new_end = find_end(text,&i,tabs,end);
				if (x==YES_WHILE){		/* Nested while loop using recursion */
					check=while_fun(new_start,new_end,text,tabs+1,print_line,argc,argv);			/* All new loops get as arguments the new limits and one extra tab */
					if ( check == ERROR ){
							return ERROR;
					}
					else if ( check == YES_BREAK ){			/* In case the nested while loop return break signal */
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
					last_if=1;
				}
				else if (x==NO_IF){
					last_if=0;
				}
				else if (x==YES_ELSE){
					if (last_if==0){
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
				else if ( x == YES_FOR ){
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
			else if ( x == SIZE_ARGUMENT || x == ARGUMENT ){
				if ( arguments(x,words,argc,argv) == ERROR ){
					return ERROR;
				}
			}
			j = 0;
			while(words[j] != NULL ){
				free(words[j++]);
			}
			free(words);
		}
		words=word_seperation(text[start]);		/* Seperate the words of the while statement */
		if ( strcmp(print_line,"-v") == 0 ){
			printf("%d: 	%s",start,text[start]);
		}
	}while ( check_cond(words) == 1);	/* Reapeat while the condition is true */
	return 0;
}
