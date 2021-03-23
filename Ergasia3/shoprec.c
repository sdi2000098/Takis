#include <stdio.h>
#include <stdlib.h>
#include "header.h"


int shop (int m, int n, int *k, int **c)
{
	int p=m,i=0;
	return calc(m, n,k, c, p, i);
}


int calc ( int m, int n, int *k, int **c, int p, int i)
{
	int j,temp,result,min;
	if ( p>= 0 && i == n){
		return p;
	}
	else if ( p<0 ){
		return m;
	}
	else if ( p >= 0 && i>= 0 && i<=n-1){
		temp=i;
		min=calc(m, n, k, c, p-c[i][0], ++temp);
		for ( j=1;j<k[i];j++ ){
			temp=i;
			result=calc(m, n, k, c, p-c[i][j], ++temp);
			if ( result < min ){
				min=result;
			}
		}
		return min;
	}
}
