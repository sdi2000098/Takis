#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

/* An algorithm for calculating complex arithmetic expressions */
/* The idea is based on the exercise 5.10 at page 166 from the book of Kernighan and Ritchie */

int precedence_bool(char words)
{
	if ( words == '+' || words == '-'){
		return 1;
	}
	else if( words == '(' || words == ')') {
		return 0;
	}
	else{
		return 2;
	}
}

struct valid expression_calculate (char **words, int i)		/* A function that deals with arithmetic operations */
{	/* Getting as arguments a line after words are seperated and an index to the first word of the expression */
	/* Return a struct type valid which consists of an int that indicates that everything is fine, an int that determines whether the expression is type int or double and a double which is the value of the expression in double type */
	struct valid temp;
	temp.ok=1;
	temp.x=0.0;
	temp.its_double = 0;
	int k,top=-1,end=-1,j;
	double part;
	char operands[100],queue[100][100];		/* operands is a stack which is going to store operands and queue is going to store both numbers and operands ( we assume that no more than 100 operands or variables is goingt to be used ) */
	extern struct double_num * ipl_double;
	extern struct var * struct_arr;
	extern int size,double_size,array_size;
	while (words[i][0] == '\t'){			/* Pass tabs */
		i++;
	}
	while ( words[i] != NULL ){
		j = 0 ;
		while (words[i][j] != '\0'){
			if ( words[i][j] == '.' ){
				temp.its_double = 1;
			}
			j++;
		}
		if ( ( (words[i][0] == '>' || words[i][0] == '<' ) && words[i][1] =='\0') || words[i][0] == '"' ||  strcmp(words[i],"!=")==0 ||  strcmp(words[i],">=")==0 || strcmp(words[i],"<=")==0 || strcmp(words[i],"!=")==0 || strcmp(words[i],"==")==0 || strcmp(words[i],"to")==0 || strcmp(words[i],"step")==0 || strcmp(words[i],"||")==0 ||  strcmp(words[i],"&&")==0){
			/* All these cases indicate that we reached the end of the expression*/
			break;
		}
		if ( (words[i][0] == '+' || words[i][0] == '-' || words[i][0] == '/' || words[i][0] == '*' || words[i][0] == '%') && words[i][1] == '\0' ){		/* It is an operand */
			if ( end == -1 ){				/* The queue that is storing variables and constants is empty */										
				fprintf(stderr,"Expected a variable or a constant before '%c'\n",words[i][0]);
				temp.ok=0;
				return temp;
			}
			if ( words[i+1] == NULL){
				fprintf (stderr,"Not enough arguments\n");
				temp.ok=0;
				return temp;
			}
			if ( top == -1 ){
				operands[++top] = words[i][0];
			}
			else{
				while ( top != -1 && ( precedence_bool(words[i][0]) < precedence_bool(operands[top]) ) ){			/* While the precedence of the operand that is going to be pushed into the stack is bigger than the top operand of the stack */ 		
					queue[++end][0] = operands[top--];									/* Pop out the operand out of the stack and insert it into the queue */
					queue[end][1] = '\0';
				}
				operands[++top] = words[i][0];						/* Push operand */
			}
		}
		else if (words[i][0] == '('){
			operands[++top] = words[i][0];				/* Push right parenthesis */
		}
		else if ( words[i][0] == ')' ){
			while(top != -1 && operands[top] != '(' ){		/* Until we find the left parenthesis */
				queue[++end][0] = operands[top--];
				queue[end][1] = '\0';
			}
			if ( top == -1 ){		/* Did not find '(' */
				break;			/* Wont print an error message because the left parenthesis we found may be part of a condition, we just know that we found all terms of the expression that needs to be calculate */	
			}
			top--;				/* Pop out left parentheseis */
		}
		else{
			strcpy(queue[++end],words[i]);		/* Its either a variable or a constant */
		}
		i++;
	} 
	while(top!= -1){
		queue[++end][0] = operands[top--];		/* Pop out all element out of stack and insert them into the queue */
		queue[end][1] = '\0';
	}
	i = 0;
	double stack[100];				/* This stack is going to store the values of the variables and constants */
	int stack_size = -1;
	while ( i <= end ){				/* Queue is not empty */
		if ( queue[i][0]=='+'){
			part = stack[stack_size-1] + stack[stack_size];			/* Add the two top elements of the stack, pop them out and push sum into the stack */
			stack_size--;
			stack[stack_size] = part;
		}
		else if ( strcmp(queue[i],"-") == 0 ){
			part = stack[stack_size-1] - stack[stack_size];
			stack_size--;
			stack[stack_size] = part;
		}
		else if ( strcmp(queue[i],"*") == 0 ){
			part = stack[stack_size-1] * stack[stack_size];
			stack_size--;
			stack[stack_size] = part;
		}
		else if ( strcmp(queue[i],"/") == 0 ){
			if (stack[stack_size] == 0 ){
				fprintf(stderr,"Cannot divide by zero\n");
				temp.ok = 0;
				return temp;
			}
			else{
				if (  temp.its_double ){
					part = stack[stack_size-1] / stack[stack_size];
				}
				else{									/* Int values, going for integer division */
					part = (int)stack[stack_size-1] / (int)stack[stack_size];
				}
				stack_size--;
				stack[stack_size] = part;
			}
		}
		else if ( strcmp(queue[i],"%") == 0 ){
			if (stack[stack_size] == 0 ){
				fprintf(stderr,"Cannot divide by zero\n");
				temp.ok = 0;
				return temp;
			}
			else{
				if ( temp.its_double ){
					fprintf(stderr,"Invalid operand to double: '%%'\n");
					temp.ok = 0;
					return temp;
				}	
				part = (int)stack[stack_size-1] % (int)stack[stack_size];
				stack_size--;
				stack[stack_size] = part;
			}
		}
		else{					/* The element is either a variable or a constant */
			stack_size++;
			/* Going to calculate the value of the variable or const */
			if ( queue[i][0] == '0' ){
				if ( atof(queue[i]) != 0 ){
					stack[stack_size] = atof(queue[i]);		/* Decimal between 0 and 1 */
				}
				else{
					stack[stack_size] = 0;
				}
			}
			else if (atof(queue[i]) != 0){
				stack[stack_size] = atof(queue[i]);
			}
			else if ( ( k = find_double(queue[i]) ) != double_size ){
				stack[stack_size] = ipl_double[k].x;
				temp.its_double=1;
			}
			else if ( check_var(queue[i]) == ITS_ARRAY){
				stack[stack_size] = (double)commands_array(queue[i],"calculate",666,NULL);
			}
			else{					/* It is either an already existing int variable or we have to create a new int */
				k=find_var(queue[i]);
				if ( k == size){		/* Did not find a variable with such name */
					if (check_var(queue[i]) == 1 ){
						if ( find_array(queue[i]) != array_size ){
							fprintf(stderr,"This name is already used for an array: %s\n",queue[i]);
							temp.ok=0;
							return temp;
						}
						struct_arr[size].x=0;		/* Since there is not a variable with such name we create one and initialize it with 0 */
						strcpy(struct_arr[size].name,queue[i]);
						size++;
						if ( ( struct_arr =(  struct var * )(realloc ( struct_arr,(size + 1 ) * sizeof ( struct var ) )  ) ) == NULL ){
							printf("Could not allocate memory \n");
							temp.ok = 0;
							return temp;
						}
						stack[stack_size] = 0.0;
					}
					else{
						temp.ok=0;
						fprintf(stderr,"Invalid input: %s\n",queue[i]);
						return temp;
					}
				}
				else {		/* There is an int variable with such name */
					stack[stack_size]=(double)struct_arr[k].x;
				}
			}
			if ( stack[stack_size] != (int)stack[stack_size] ){
				temp.its_double = 1;
			}
		}
		i++;
	}
	if ( stack_size != 0 ){				/* Ran out of operands but there are variable remainding */
		fprintf(stderr,"Invalid input: ");
		i = 0 ;
		while ( words[i] != NULL ){
			fprintf(stderr,"%s ",words[i++]);
		}
		putchar('\n');
		temp.ok=0;
		return temp;
	}
	temp.x = stack[0];
	return temp;
}
		

















