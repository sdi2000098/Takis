#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

/* An algorithm for calculating complex conditions */
/* The idea is based on the exercise 5.10 at page 166 from the book of Kernighan and Ritchie */

int precedence(char words)
{
	if ( words == '|'){
		return 1;
	}
	else if( words == '(' || words == ')') {
		return 0;
	}
	else{
		return 2;
	}
}

int check_cond (char ** words)		/* Function that checks a condtion, returns 1 in case of true and 0 in case of false */
{	/* Getting as arguments a line after words are seperated */
	extern struct double_num * ipl_double ;	
	int j,i,top=-1,end=-1;
	double x = 0,y=0;
	char operator[100],queue[100][100];		/* operator is a stack to store operator and queue is going to store variables and operator */
	extern struct var  * struct_arr;
	extern int size,double_size;
	struct valid temp;
	int part;
	if ( words[0][0] == '\t' ){		/* Pass tabs */
		i=1;
	}
	else{
		i=0;
	}
	if (strcmp(words[i],"while") == 0 || strcmp(words[i],"if") == 0){		/* Pass while and if */
		i++;
	}
	while ( words[i] != NULL ){
		if ( !strcmp(words[i],"&&") || !strcmp(words[i],"||")  ){
			if ( end == -1 ){
				fprintf(stderr,"Expected a conditional statement before '%c'\n",words[i][0]);
				return ERROR;
			}
			if ( words[i+1] == NULL){
				fprintf(stderr,"Not enough arguments\n");
				return ERROR;
			}
			if ( top == -1 ){			/* Push into the stack | or & */
				operator[++top] = words[i][0];
			}
			else{
				while ( top != -1 && ( precedence(words[i][0]) < precedence(operator[top]) ) ){			/* While the precedence of the opertor that is going to be pushed into the stack is bigger than the top operatgor of the stack */
					queue[++end][0] = operator[top--];			
					queue[end][1] = '\0';						/* Pop out the operator and insert it into the queue */
				}
				operator[++top] = words[i][0];					/* Push the new operator */
			}
			i++;
		}
		else if (words[i][0] == '('){
			operator[++top] = words[i++][0];
		}
		else if ( words[i][0] == ')'){
			while(top != -1 && operator[top] != '(' ){			/* Pop out all operator and insert them to the queue untill we find left parenthesis */
				queue[++end][0] = operator[top--];
				queue[end][1] = '\0';
			}
			if ( top == -1 ){
				fprintf(stderr,"Expected '(' \n");
				temp.ok=0;
				return ERROR;
			}
			top--;				/* Pop out left parenthesis */
			i++;
		}
		else{
			temp=expression_calculate(words,i);		/* Calculate the value of the first part of the condition */
			if ( !temp.ok ){
				return ERROR;
			}
			x=temp.x;
			while ( check_var(words[i]) != ERROR || atof(words[i]) != 0 ||  words[i][0] == '\0' || (  (words[i][0] == '+' || words[i][0] == '-' || words[i][0] == '/' || words[i][0] == '*' || words[i][0] == '%' || words[i][0] == '(' || words[i][0] == ')' ) && words[i][1] == '\0')){
				i++;
				/* Find the start of the second part */
			}
			i++;
			temp=expression_calculate(words,i);		/* Calculate the value of the second part */
			if ( !temp.ok ){
				return ERROR;
			}
			y=temp.x;
			i--;					/* Find the operator and do the comparison */
			if ( strcmp(words[i],"==") == 0){
				if (x == y){
					queue[++end][0] = '1';		/* Since the condition is true insert to the queue the value 1 as a string */
					queue[end][1] = '\0';
				}
				else{
					queue[++end][0] = '0';
					queue[end][1] = '\0';

				}
			}
			else if ( strcmp(words[i],"!=")  == 0){
				if (x != y){
					queue[++end][0] = '1';
					queue[end][1] = '\0';
				}
				else{
					queue[++end][0] = '0';
					queue[end][1] = '\0';
				}
			}
			else if ( strcmp(words[i],"<=")  == 0){
				if (x <= y){
					queue[++end][0] = '1';
					queue[end][1] = '\0';
				}
				else{
					queue[++end][0] = '0';
					queue[end][1] = '\0';

				}
			}
			else if ( strcmp(words[i],"<")  == 0){
				if (x < y){
					queue[++end][0] = '1';
					queue[end][1] = '\0';
				}
				else{
					queue[++end][0] = '0';
					queue[end][1] = '\0';
				}
			}
			else if (strcmp(words[i],">=")  == 0 ){
				if (x >= y){
					queue[++end][0] = '1';
					queue[end][1] = '\0';
				}
				else{
					queue[++end][0] = '0';
					queue[end][1] = '\0';
				}
			}
			else if ( strcmp(words[i],">")  == 0 ){
				if (x > y){
					queue[++end][0] = '1';
					queue[end][1] = '\0';
				}
				else{
					queue[++end][0] = '0';
					queue[end][1] = '\0';
				}
			}
			else {
				fprintf (stderr,"Invalid input: %s\n",words[i]);
				return ERROR;
			}
			while ( words[i] != NULL && words[i][0]!='&' && words[i][0] != '|' && words[i][0] != ')'){		/* Pass all tokens until we reach end or the beginning of another term of the condtion */
				i++;
			}
		}
	}
	while(top != -1){				/* Pop out all operators and insert them to the queue */
		queue[++end][0] = operator[top--];
		queue[end][1] = '\0';
	}
	i = 0;
	int stack[100];			/* This stack is going to store the values of all conditional statements */
	int stack_size = -1;
	while ( i <= end ){				/* While there are more elements in the queue */
		if ( queue[i][0]=='&'){
			part = stack[stack_size-1] * stack[stack_size];			/* Boolean and is equivalent to multiplication betweene the two top elements of the stack */
			stack_size--;							/* Pop out the two top elements */
			stack[stack_size] = part;					/* Push the result of the multiplication */
		}
		else if ( strcmp(queue[i],"|") == 0 ){
			part = stack[stack_size-1] + stack[stack_size];			/* Boolean or is equivalent to multiplication */
			stack_size--;
			stack[stack_size] = part;
		}
		else{
			stack[++stack_size] = atoi(queue[i]);				/* Convert the string containing the value of the condition into an int value and push it to the stack */
		}
		i++;
	}
	return stack[0];			/* Return the final value of the whole condition */
}








