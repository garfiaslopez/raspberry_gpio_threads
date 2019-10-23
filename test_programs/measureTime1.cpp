/******************************************************************************
* FILE: thread17.cpp
* DESCRIPTION:
* This program shows how to measure execution time through clock()
* function from time.h head file
*
* Notes:
*
* ISO/IEC 9899:1990 7.12.1: <time.h>
* The macro `CLOCKS_PER_SEC' is the number per second of the value
* returned by the `clock' function.
* CAE XSH, Issue 4, Version 2: <time.h>
* The value of CLOCKS_PER_SEC is required to be 1 million on all
* XSI-conformant systems.
* #  define CLOCKS_PER_SEC  1000000l
*
* AUTHOR: Conejo
* DATE:   March 12, 2013
******************************************************************************/

#include <pthread.h>
#include <time.h>		/* clock_t, clock, CLOCKS_PER_SEC */
#include <stdio.h>      /* printf */
#include <math.h>       /* sqrt   */

// The value of CLOCKS_PER_SEC is required to be 1 million on all
// XSI-conformant systems.
#define CLOCKS_PER_SEC 1000000l

/* Calculate number of primes from zero to n */
int frequency_of_primes (int n) {
	int i,j;
	int freq=n-1;
	for (i=2; i<=n; ++i) for (j=sqrt(i);j>1;--j) if (i%j==0) {--freq; break;}
	return freq;
} // frequency_of_primes function end

int main (int argc, char** argv) {
	clock_t t;
	int f;
	t = clock();
	printf("Calculating...\n");
	f = frequency_of_primes(99999);
	printf("The number of primes lower than 100,000 is: %d\n",f);
	t = clock() - t;
	// printf();
	printf("It took me %i clicks (%f seconds).\n",(int)t,((float)t)/CLOCKS_PER_SEC);
	return 0;
} // main function end
