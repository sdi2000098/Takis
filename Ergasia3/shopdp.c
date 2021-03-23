#include <stdio.h>
#include <stdlib.h>
#include "header.h"
//#define COSTS

int shop(int m, int n, int *k, int **c)
{
	int t,i,j,**array;
	array=malloc(n*sizeof(int*));
	if (array == NULL ){
		printf("Sorry cannot allocate memory\n");
		return ERROR;
	}
	for (i=0;i<n;i++){
		array[i]=malloc((m+1)*sizeof(int));
		if (array[i] == NULL ){
	                printf("Sorry cannot allocate memory\n");
        	        return ERROR;
      	        }
		for (j=0;j<m+1;j++){
			array[i][j]=0;
		}
	}
	for (i=0;i<n;i++){
		for (j=0;j<k[i];j++){
			if ( m-c[i][j] >= 0){         /* We need to be within the limits of the array */
				if ( i == 0){
					array[i][m-c[i][j]]= 1;
				}
				else {
					for (t=0;t<m+1;t++){
						if (array[i-1][t] == 1){
							if ( t-c[i][j] >= 0){
								array[i][t-c[i][j]]=1;
							}
						}
					}
				}
			}
		}
	}
	#ifdef COSTS
		printf ( "Item costs: ");
		int amount=0,*model;
		model=malloc(n*sizeof ( int ));
		if ( model == NULL ){
			printf("Sorry cannot allocate memory\n");
                	return ERROR;
        	}
		for ( i=n-1;i>=0;i--){
			if ( i == n-1){
				for ( j=0;j<m+1;j++){
					if ( array[i][j] == 1 ){
						amount =j;              /* Find the minimum amount of money left */
						break;
					}
				}
			}
			else {
				for ( j=0;j<k[i+1];j++){
                    if ( amount + c[i+1][j] < m+1){
                        if ( array[i][amount+c[i+1][j]] == 1 ){         /* Check if we can reach the previous amount of money buying the model */
                            model[i+1]=j;                               /* Save the model */
                            amount+=c[i+1][j];
                            break;
                        }
                    }
                }
            }
        }
		for ( j=0;j<k[0];j++ ){
			if ( amount + c[0][j] == m ){               /* Check which model of the first product we have to buy */
				model[0]=j;
				break;
			}
		}
		for ( i=0;i<n;i++){
			printf ( "%d ",c[i][model[i]]);
		}
		putchar('\n');
		free(model);
	#endif
	for ( i=0;i<m+1;i++){
			if ( array[n-1][i] == 1){
				return i;
		}
	}
	return m;
}
