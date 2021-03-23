#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

int if_fun (int start,int end,char **text, int tabs,char * print_line,int argc, char* argv[])		/* Function that deals with if statements */
{	
	int i,x,new_start,new_end,count_tabs,j,last_if=0,check;	
	char **words;
	for (i=start+1;i<=end;i++){					/* For each line */
		if ( (words=word_seperation(text[i]) ) == NULL){
			fprintf(stderr,"Could not allocate memory\n");
			return ERROR;
		}
		if ( strcmp(print_line,"-v" ) == 0 ){
			printf("%d:	",i);
		}
		x=commands(words,tabs,print_line);
		if ( x == ERROR){
			return ERROR;
		}
		else if ( x== 0 ){
			continue;
		}
		else if ( x==YES_CONTINUE || x==YES_BREAK ){		/* Return signal to break or continue at the nested loop */
			return x;
		}
		else if ( x == YES_WHILE || x==NO_WHILE || x == YES_IF || x==NO_IF || x==YES_ELSE || x == YES_FOR){
			new_start=i;
			new_end=find_end(text,&i,tabs,end);		/* Find new limits */
			if (x==YES_WHILE){
				check=while_fun(new_start,new_end,text,tabs+1,print_line,argc,argv);
				if ( check == ERROR ){
					return ERROR;
				}
				else if ( check == YES_BREAK ){
					return YES_BREAK;
				}
				else if ( check == YES_CONTINUE ){
					return YES_CONTINUE;
				}
			}
			else if (x==YES_IF){
				check = if_fun(new_start,new_end,text,tabs+1,print_line,argc,argv) ;
				if ( check == ERROR ){
					return ERROR;
				}
				else if ( check == YES_BREAK ){
					return YES_BREAK;
				}
				else if ( check == YES_CONTINUE ){
					return YES_CONTINUE;
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
						return YES_BREAK;
					}
					else if ( check == YES_CONTINUE ){
						return YES_CONTINUE;
					}
				}
			}
			else if ( x == YES_FOR ){
				check=for_fun(new_start,new_end,text,tabs+1,print_line,argc,argv);
				if ( check == ERROR ){
					return ERROR;
				}
				else if ( check == YES_BREAK ){
					return YES_BREAK;
				}
				else if ( check == YES_CONTINUE ){
					return YES_CONTINUE;
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
	return 0;
}
