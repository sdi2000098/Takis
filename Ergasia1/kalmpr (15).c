#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MINUM 2100000000
#define MAXNUM 2105000000
#define MAXK 5
#define COMPUTATIONS 20

int main()
{   int countk,count2,current,temp,factors,divisor,prime,prime_2,semiprime1,semiprime2,possible_prime,x,y,z,i;
    long curtime;
    countk = 0; 	                  /* The number of k-almost primes*/
    count2 = 0;         	          /* The numbers of semiprimes*/
    printf ("Checking numbers in [%d,%d]\n\n",MINUM,MAXNUM);
    for (current = MINUM ; current <= MAXNUM ; current++){          /*Factorize each number */
        semiprime1=0;                    /* The variable to save the first factor of a semiprime */
        semiprime2=0;                    /* The variable to save the second factor of a semiprime */
        factors = 0;                     /* The number of prime factors of each current number */
        temp=current;                    /*Save the value of current number so not to change the current number */
        while (temp % 2 == 0){
            temp = temp / 2;             /* Factorize by 2 as many times as possible */
            factors++;
            if (semiprime1 == 0 ){
                semiprime1 = 2;
            }
            else if ((semiprime1 != 0) && (semiprime2 == 0)){
                semiprime2 = 2;
            }
        }
        while (temp % 3 == 0){
            temp = temp / 3;            /* factorize by 3 as many times as possbile */
            factors++;
            if (semiprime1 == 0 ){
                semiprime1 = 3;
            }
            else if ((semiprime1 != 0) && (semiprime2 == 0)){
                semiprime2 = 3;
            }
        }
                possible_prime = 5;        /*The possible factor of current number
 			                   we are going to divide by it only if we find out that is prime */
                while ((temp != 1) && (factors < MAXK) && (possible_prime*possible_prime<=temp)  ){
                /* Continue only if there are more prime factors and the number of prime factors have not exceeded the limit*/
                if (temp % possible_prime == 0){
                /*Going to check whether the possible_prime is prime, only if it is factor of our number */
                    prime = 1;            /* Determines whether the possible_prime is prime or not */
                    divisor = 5;
                    while ((prime == 1 ) &&  (divisor*divisor <= possible_prime)){
                    /* Since the current number cannot be divided by 2 or 3,
                    we are going to discard the possible_primes that have 2 or 3 as factors*/
                        if ((possible_prime % divisor == 0) || (possible_prime % (divisor + 2) == 0)){
                            /* We will check divisors of the possible_prime that cannot be divided by 2 or 3 */
                            prime = 0;
                        }
                        divisor +=6;               /* Going for the next possible divisors of the possible_prime */
                    }
                    if (prime == 1){
                        while (temp % possible_prime == 0){
                        /* Since we found a prime, we are going to factorize by it as many times as possible */
                            temp = temp / possible_prime;
                            factors ++;
                            if (semiprime1 == 0 ){
                                semiprime1 = possible_prime;
                            }
                            else if ((semiprime1 != 0) && (semiprime2 == 0)){
                                semiprime2 = possible_prime;
                            }
                        }
                    }
                }
                if ((temp != 1) && (factors < MAXK) && ( temp %(possible_prime +2) == 0  )){
                /* Since the number need further factorization and the factors have not exceeded the limit
                and the possible_prime + 2 is factor of current number */
                    prime_2 = 1;
                    divisor = 5;
                    while ((prime_2 == 1) && (divisor*divisor<=(possible_prime+2))){
                    /* Going to check whether possible_prime + 2 is prime or not */
                        if (((possible_prime +2) % divisor == 0) || ((possible_prime + 2) % (divisor + 2) == 0)){
                        /*Means that possible_prime + 2 is not a prime*/
                            prime_2=0;
                        }
                        divisor +=6;
                    }
                    if (prime_2 == 1){
                        while ((temp % (possible_prime +2) == 0) ){
                            temp = temp / (possible_prime +2);
                            factors++;
                            if (semiprime1 == 0 ){
                                semiprime1 = (possible_prime + 2);
                            }
                            else if ((semiprime1 != 0) && (semiprime2 == 0)){
                                semiprime2 = (possible_prime + 2);
                            }
                        }
                    }
                }
                possible_prime +=6 ;
            }
            if ((temp != 1) && (factors < MAXK) ){          /*Means that temp is a prime number */
                factors++;

                if (semiprime1 == 0 ){
                semiprime1 = temp;
                }
                else if ((semiprime1 != 0) && (semiprime2 == 0)){
                    semiprime2 = temp;
                }
                temp=1;
            }
        if ((factors == 2) && (temp==1)){             /* The number is semiprime and we are done with factorizing it */
            count2++;
            countk++;
            if ((semiprime1 != semiprime2) && ((double) semiprime1 > ((double) semiprime2 - (double) semiprime2*0.01))){
            /* Checking whether the semiprime is interesting or not. Also converting integers to doubles in order to make the operation.*/
               printf ("%d * %d = %d is an interesting semiprime\n",semiprime1,semiprime2,current);
            }
        }
        else if ((factors <= MAXK )  && (factors>=2) && (temp == 1)) {
            countk++;
        }
    }
    printf ("\nFound %d k-almost prime numbers (2 <= k <= %d)\nFound %d semiprimes\n",countk,MAXK,count2);
    curtime=time(NULL);
    srand((unsigned int ) curtime);
    printf("\nCurrent time is %ld\n\nChecking %d random numbers\n\n",curtime,COMPUTATIONS);
    for (i=1;i<=COMPUTATIONS;i++){              /* The number of number that are going to be checked */
        y=rand();
        z=rand();
        x=((y % 32768) + 1 )*((z % 32768) + 1) +1;
        temp=x;             /* Saving the value of x so not to lose the number */
        factors=0;
        while (temp % 2 == 0){              /* Factorize by 2 as many times as possivle*/
            temp = temp / 2;
            factors++;
        }
        while (temp % 3 == 0){              /* Factorize by 3 as many times as possible */
            temp = temp / 3;
            factors++;
        }
        possible_prime = 5;        /*The possible factor of current number
       				   We are going to divide by it only if we find out that is prime */
        while ((temp != 1)  && (possible_prime*possible_prime<=temp)  ){
        /* Continue only if there are more prime factors */
            if (temp % possible_prime == 0){
            /*Going to check whether the possible_prime is prime, only if it is factor of our number */
                prime = 1;            /* Determines whether the possible_prime is prime or not */
                divisor = 5;
                while ((prime == 1 ) &&  (divisor*divisor <= possible_prime)){
                /*Checking for divisors equal or smaller than the square root of the number
                Since the current number cannot be divided by 2 or 3,
                we are going to discard the possible_primes that have 2 or 3 as factors*/
                    if ((possible_prime % divisor == 0) || (possible_prime % (divisor + 2) == 0)){
                    /* We will check divisors of the possible_prime that cannot be divided by 2 or 3 */
                        prime = 0;
                        }
                    divisor +=6;               /* Going for the next possible divisors of the possible_prime */
                }
                if (prime == 1){
                    while (temp % possible_prime == 0){
                    /* Since we found a prime, we are going to factorize by it as many times as possible */
                        temp = temp / possible_prime;
                        factors ++;
                    }
                }
            }
            if ((temp != 1)  && ( temp %(possible_prime +2) == 0  )){
            /* Since the number need further factorization
            and the possible_prime + 2 is factor of current number */
                prime_2 = 1;
                divisor = 5;
                while ((prime_2 == 1) && (divisor*divisor<=(possible_prime+2))){
                /* Going to check whether possible_prime + 2 is prime or not */
                    if (((possible_prime +2) % divisor == 0) || ((possible_prime + 2) % (divisor + 2) == 0)){
                    /* That means that possible_prime + 2 is not a prime*/
                        prime_2=0;
                    }
                    divisor +=6;
                }
                if (prime_2 == 1){
                    while (temp % (possible_prime +2) == 0){
                        temp = temp / (possible_prime +2);
                        factors++;
                    }
                }
            }
            possible_prime+=6;
        }
        if(temp != 1) {          /* Means that the number is prime */
            factors++;
        }
        printf("%10d is a %d-almost prime number\n",x,factors);
    }
}
