#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"



int find_end (char ** text, int * i , int tabs, int end)		/* A function that return the end of if, for or while loop */
{	/* Getting as arguments the text in which each line of the program is stored, a pointer to the line in which the loop starts, the amount of tabs used in the loop and end which indicates the limit */
	int new_end, count_tabs, new_start=*i,j,k;
	char ** words;
	++(*(i));		/* Go fo the next line */
	for (; *i <= end ;++(*(i))){
		count_tabs = 0;
		if ( (words=word_seperation(text[*i]) ) == NULL){		/* Seperate words */
			fprintf(stderr,"Could not allocate memory\n");
			return ERROR;
		}
		if (words[0] != NULL ){
			j=0;
			while ( words[0][j] =='\t'){			/* Count tabs */
				count_tabs++;
				j++;
			}
			if (count_tabs < tabs+1){			/* Previous line was the end */
				new_end= *i-1;
				return new_end;
			}
			if ( *i == end ){			/* Reached the limit */
				return end;
			}
		}
		else{				/* The line might be either empty or full of comments */
			new_end = *i-1;		/* Save the previous line */
			j = *i;
			while ( words[1][0] == '#'){		/* Pass all lines tha are full of comments */
				j++;
				if ( (words=word_seperation(text[j]) ) == NULL){
					fprintf(stderr,"Could not allocate memory\n");
					return ERROR;
				}
			}
			if (words[0] != NULL ){		/* This line is neither empty nor full of comments */
				k=0;
				count_tabs=0;
				while ( words[0][k] =='\t'){
					count_tabs++;
					k++;
				}
				if ( count_tabs >= tabs + 1){		/* Continue the procedure of finding the end of loop */
					*i = j-1;
					continue;
				}
			}
			return new_end;			/* In this case after passing lines full of comments or empty ones, we found a line with less comments than expected. Hence the loop ends in the line which is right before we start passing lines */
		}
		j = 0;
		while ( words[j] != NULL ){
			free(words[j++]);
		}
		free(words);
	}
}