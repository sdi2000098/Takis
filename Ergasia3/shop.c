#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char *argv[])
{
    int s,amount, n, m, *k, i, **c, j;
    if (argc =! 2){
        return ERROR;
    }
    m=atoi(argv[1]);
    if (scanf("%d",&n) != 1){       /* Read number of products */
        printf("Wrong input.\n");
        return ERROR;
    }
    k=malloc(n*sizeof(int));        /* Allocate memory to store the number of models for each product */
    if ( k == NULL){
        printf("Sorry cannot allocate memory. \n");
        return ERROR;
    }
    c=malloc(n * sizeof(int *));
    if (c == NULL){
        printf("Sorry cannot allocate memory. \n");
        return ERROR;
    }
    for (i=0;i<n;i++){
        if (scanf("%d",&k[i]) != 1){        /* Read the number of models for each product */
            printf("Wrong input.\n");
            return ERROR;
        }
        *(c+i) = malloc(k[i] * sizeof(int));
        if (*(c+i) == NULL){
            printf("Sorry cannot allocate memory. \n");
            return ERROR;
        }
        for (j=0;j<k[i];j++){
            if (scanf("%d",&c[i][j]) != 1){
                printf("Wrong input.\n");
                return ERROR;
            }
        }
    }
    amount=shop(m,n,k,c);
    if ( amount == ERROR ){
	return -1;
    }
    s=m-amount;
    if (s == 0 ){
	printf("No solution found\n");
    }
    else{
	printf("Maximum amount spent is: %d\n",s);
    }
    free(k);
   for (i=0;i<n;i++){
	free(c[i]);
   }
   free(c);
   return 0;
}
