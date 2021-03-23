#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "variables.h"

struct ipl_arrays ** all_arrays = NULL;		/* Will be used to dynamically allocate memory for the structs that are going to store the arrays */

int array_size=0;				/* Indicates the number of arrays */

int create_array(char ** words,int i)			/* A function to create a new array */
{	/* Getting as arguments a specific line after words are seperated and  an index to find the name of the array */
	extern struct var * struct_arr;
	extern int size;
	char name_of_array[100],size_of_array[100];
	int j=0,k=0;
	while (words[i][j] != '['){			/* Everything before '[' is the array name */
		name_of_array[j]= words[i][j];
		j++;
	}
	name_of_array[j]='\0';
	if ( check_var(name_of_array) == ERROR){	/* Invalid name */
		return ERROR;
	}
	if ( find_var(name_of_array) != size ){
		fprintf(stderr,"%s is already a name of a variable\n",name_of_array);
		return ERROR;
	}
	if (find_array(name_of_array) != array_size){
		fprintf(stderr,"%s is already a name of array\n",name_of_array);
		return ERROR;
	}
	j++;
	while (words[i][j] != ']'){			/* Size of array is everything between '[' and ']' */
		size_of_array[k++]=words[i][j++];
	}
	size_of_array[k]='\0';
	if ( ( all_arrays = (struct ipl_arrays**)(realloc(all_arrays,(array_size + 1)*sizeof(struct ipl_arrays*) )) )== NULL){
		/* Allocating memory for one more pointer to struct ipl_arrays */
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	if ( (all_arrays[array_size] = (struct ipl_arrays*)(malloc(sizeof (struct ipl_arrays) ) ) ) == NULL ){
		/* Allocating memory for the new struct ipl_arrays */
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	strcpy(all_arrays[array_size]->name,name_of_array);
	if (size_of_array[0] == '0' && atoi(size_of_array) == 0){
		fprintf(stderr,"Invalid size of array");
		return ERROR;
	}
	else if ( atoi(size_of_array) != 0){		/* Store to the new struct the size of the array */
		all_arrays[array_size]->dim=atoi(size_of_array);
	}
	else if ( (k=find_var ( size_of_array)) != size){
		all_arrays[array_size]->dim= struct_arr[k].x;
	}
	else{
		fprintf(stderr,"Invalid size of array");
		return ERROR;
	}
	if ( ( (all_arrays[array_size])->array= (int *) (malloc( (all_arrays[array_size]->dim) * sizeof(int) )) )== NULL ) {
		/* Allocate memory for the ipl array */
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	for ( j = 0 ;j <all_arrays[array_size]->dim ; j++ ){
		/* Initialize all elements of the array as zero */
		all_arrays[array_size]->array[j] = 0;
	}
	array_size++;
	return 0;
}


int free_array ( char ** words, int i)			/* Function that free the allocated memory for the array */
{	/* Getting as arguments a specific line after word seperation and and index to find the name of the array */
	extern struct ipl_arrays ** all_arrays ;
	extern int array_size,size;
	int k;
	k = find_array(words[i]);
	if (k == array_size){
		fprintf(stderr,"There is not an array with such name: %s\n",words[i]);
		return ERROR;
	}
	array_size--;
	free( all_arrays[k]->array );			/* First free the memory allocated for the ipl arra */
	free(all_arrays[k]);
	if ( ( all_arrays = (struct ipl_arrays**)(realloc(all_arrays,(array_size + 1)*sizeof(struct ipl_arrays*) ) ) ) == NULL ){
		fprintf(stderr,"Could not allocate memory\n");
		return ERROR;
	}
	return 0;
}


int commands_array (char * words,char commands[20],int x,int ** point_int)		/* A function that deals with array operations */
{	/* Getting as arguments the name of the element, the command to be performed */
	/* Int x is used in case we have to insert something into the element,  int ** point_int is used as pointer in case we need to return the addres of the element */
	extern int array_size;
	extern struct ipl_arrays ** all_arrays ;
	char array_name[100],size_of_array[100];
	int j = 0, k=0;
	while (words[j] != '['){			/* Find the name of the array */
		array_name[k++]=words[j++];
	}
	array_name[k] = '\0';
	j++;
	k=0;
	while (words[j] != ']' && words[j] != '\0'){	/* Find the index */
		size_of_array[k++]=words[j++];
	}
	size_of_array[k]='\0';
	j=find_array(array_name);
	if ( j == array_size){
		fprintf(stderr,"There is no array with such name: %s",array_name);
		return ERROR;
	}
	if ( (k = array_index(size_of_array) ) == ERROR ){
		return ERROR;
	}
	if ( k < 0 || k >=all_arrays[j]->dim ) {
		printf("Invalid index '%d'\n",k);
		return ERROR;
	}
	if ( strcmp(commands,"read") == 0){		/* Read a value and insert it  */
		if ( scanf("%d",&(all_arrays[j]->array[k])) != 1){
			fprintf(stderr,"Expected integer\n");
			return ERROR;
		}
	}
	else if (strcmp(commands,"write") == 0){	/* Print the element */
		printf("%d ",all_arrays[j]->array[k]);
	}
	else if (strcmp(commands,"writeln") == 0){	
		printf("%d\n",all_arrays[j]->array[k]);
	}
	else if (strcmp(commands,"insert") == 0){		/* Insert to elemnt the value of x */
		all_arrays[j]->array[k]=x;
	}
	else if (strcmp(commands,"random") == 0 ){		/* Insert a random value  */
		all_arrays[j]->array[k]=rand();
	}
	else if (strcmp(commands,"calculate") == 0 ){		/* Rerurn the value of the elemnt */
		return all_arrays[j]->array[k];
	}
	else if (strcmp(commands,"address") == 0 ){		/* Return the addres  of the elemnt */
		*point_int=&(all_arrays[j]->array[k]);
	}
	return 0;
}


int array_index (char words[100] )
{	/* Getting as argument whatever is between '[' and ']' and returns index */
	extern int size;
	extern struct var * struct_arr;
	int i;
	if ( words[0] == '0'){
		return 0;
	}
	else if ( atoi(words) != 0 ){
		return atoi(words);
	}
	else if ( ( i = find_var(words) ) != size ){
		return struct_arr[i].x;
	}
	else if ( check_var(words) == ITS_ARRAY ){
		i = commands_array(words,"calculate",666,NULL);
		return i;
	}
	else{
		fprintf(stderr,"Invalid index\n");
		return ERROR;
	}
}










