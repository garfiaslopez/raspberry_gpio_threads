/******************************************************************************
* FILE: thread18.cpp
* DESCRIPTION:
* This program calculates the time required to
* execute the program specified as its first argument.
* The time is printed in seconds, on standard out.
*
* This program uses the POSIX standard function clock_gettime()
* to get the time from system against clock() function
*
* Notes:
*
* Compile with -lrt linker option which is the library that contains implementation
* objects
*
* The Identifiers defined in bits/time.h header file is not detected so
* it's necessary to define again into program as you can find below
*
* AUTHOR: Conejo
* DATE:   March 12, 2013
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/* Identifier for system-wide realtime clock. */
#define CLOCK_REALTIME		0
/* Monotonic system-wide clock. */
// #define CLOCK_MONOTONIC		1
/* High-resolution timer from the CPU. */
// #define CLOCK_PROCESS_CPUTIME_ID	2
/* Thread-specific CPU-time clock. */
// #define CLOCK_THREAD_CPUTIME_ID	3
/* Monotonic system-wide clock, not adjusted for frequency scaling. */
// #define CLOCK_MONOTONIC_RAW		4
/* Identifier for system-wide realtime clock, updated only on ticks. */
// #define CLOCK_REALTIME_COARSE	5
/* Monotonic system-wide clock, updated only on ticks. */
// #define CLOCK_MONOTONIC_COARSE	6
/* Monotonic system-wide clock that includes time spent in suspension. */
// #define CLOCK_BOOTTIME		7
/* Like CLOCK_REALTIME but also wakes suspended system. */
// #define CLOCK_REALTIME_ALARM		8
/* Like CLOCK_BOOTTIME but also wakes suspended system. */
// #define CLOCK_BOOTTIME_ALARM		9

#define BILLION  1000000000L;

int main( int argc, char** argv )
  {
	/*
	 * Data structure defined in bits/types.h which has two
	 * time_t elements: tv_sec and tv_nsec. This elements contain
	 * the time in nanoseconds. If you want to
	 * know the total time in seconds, the tv_sec and (tv_nsec/1E-9)
	 * must be added
	*/
    struct timespec start, stop;

    double accum;

    if(clock_gettime(CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    // Execute the given line as a shell command
    // If is not given, a space is assumed
    system( argv[1] );

    if(clock_gettime(CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    accum = ( stop.tv_sec - start.tv_sec )
             + (double)( stop.tv_nsec - start.tv_nsec )
               / (double)BILLION;
    printf( "Time duration:  %0.8f\n", accum );
    return EXIT_SUCCESS;
  }
