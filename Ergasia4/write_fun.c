#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"


int write_fun (char ** words,char * command,int i)
{	
	int k;
	struct valid temp;
	while ( words[i] != NULL ){
		if ( words[i][0] == '"' ){				/* It is a string */
			k = 1;
			while ( words[i][k] != '"' ){
				if ( words[i][k] == '\\' ){
					switch(words[i][++k]){
						case 'a': 
							putchar('\a');
							break;
						case 'b':
							putchar('\b');
							break;
						case 'n': 
							putchar('\n');
							break;
						case 't': 
							putchar('\t');
							break;
						defasult:
							fprintf(stderr,"Invalid escape sequence\n");
							break;
					}
					k++;
				}
				else{
					putchar(words[i][k++]);
				}
			}
			i++;
		}
		else{
			temp = expression_calculate(words,i);
			if (!temp.ok){
				return ERROR;
			}
			if (temp.its_double){
				printf("%lf",temp.x);
			}
			else{
				printf("%d",(int)temp.x);
			}
			while ( words[i] != NULL && words[i][0] != '"' ){
				i++;
			}
		}
	}
	printf("%s",strcmp(command,"write") ? "\n" : " " );
	return 0;
}