#include <stdio.h>
#include <stdlib.h>
#include "header.h"
//#define COSTS

int shop ( int m, int n, int *k, int **c)
{
	int i,result,j,**array,p=m;
	array=malloc( ( n+1) * sizeof(int *));
	if (array == NULL ){
		printf("Sorry cannot allocate memory\n");
		return ERROR;
	}
	for (i=0;i<n+1;i++){
		*(array + i)=malloc((m+1)*sizeof(int));
		if (*(array + i) == NULL){
			printf("Sorry cannot allocate memory\n");
	               return ERROR;
		}
		for (j=0;j<m+1;j++){
			array[i][j]=-1;             /* The value -1 means that we have not calculated this R{i}{j} */
		}
	}
	result=calcmem(m,n,k,c,p,0,array);
	#ifdef COSTS
		int cost,model,min;
		printf("Items costs: ");
		p=m;
		for ( i=1;i<n+1;i++){
			min=m;
			for ( j=0;j<m+1;j++ ){
				if ( array[i][j] < min && array[i][j] != -1){           /* Find the model of the product that results to minimum amount of money left, and print the cost of the model */
					min=array[i][j];
					model=j;
				}
			}
			cost=p-model;
			printf("%d ",p-model);
			p-=cost;
		}
		putchar('\n');
	#endif
	for (j=0;j<n+1;j++){
                free(array[j]);
        }
	free(array);
	return result;
}


int calcmem (int m, int n, int *k, int **c, int p, int i, int **array)
{
	int min,temp,j;
	if (p >= 0 && i == n) {
		return p;
	}
	else if (p<0){
		return m;
	}
	else if ( p >= 0 && i >= 0 && i<=n-1){
		min=m;
		for (j=0;j<k[i];j++){
			 if ( p-c[i][j] >= 0){
				if ( array[i+1][p-c[i][j]] == -1){          /* We need to calculate it */
					array[i+1][p-c[i][j]] = calcmem(m,n,k,c,p-c[i][j],i+1,array);
				}

				if ( array[i+1][p-c[i][j]] < min ){
					min=array[i+1][p-c[i][j]];
				}
			}
		}
		return min;
	}
}
