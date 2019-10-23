/******************************************************************************
* FILE:		thread16.cpp
* DESCRIPTION:
* This program shows how to use CPU affinity policy
*
* AUTHOR:	Conejo
* DATE:		Oct 25, 2012
*
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>			//Inlcude sleep () function

#define NUM_THREADS	2

// Funcion cuya tarea es mostrar en consola un mensaje
void *functiont(void *threadid)
{
	int rc;
	cpu_set_t cpuset;
	long tid;
	tid = (long)threadid;
	printf("Thread #%ld: starting...\n", tid);
	rc = pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    printf("Thread #%ld: with affinity:\n",tid);
    for (int i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpuset))
            printf("\t\tCPU %d\n", i);
	sleep(3);
	printf("Thread #%ld: finishing...\n", tid);
	pthread_exit((void*)tid);
}

int main(int argc, char *argv[]) {

	int rc, detachstate, policy, priority;
	int affinity;
	long t;
	void *status;

	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	sched_param param;

	// Affinity variables
    cpu_set_t cpuset;

    // Initialize and set thread's attributes
    pthread_attr_init(&attr);

    // Get detach state and it's stored into detachstate variable1
    // PTHREAD_CREATE_JOINABLE = 0
    // PTHREAD_CREATE_DETACHED = 1
    rc = pthread_attr_getdetachstate(&attr,&detachstate);

    // Get initial policy scheduling parameter and is stored into policy variable
    rc = pthread_attr_getschedpolicy(&attr,&policy);

    // Get initial priority scheduling parameter and is stored into struct param
    rc = pthread_attr_getschedparam(&attr,&param);
    priority=param.sched_priority;

    // Get the initial CPU affinity mask attribute and is stored in cpuset
    rc = pthread_attr_getaffinity_np(&attr,sizeof(cpu_set_t), &cpuset);

    // Print default attributes
    printf("Default attributes:\n");
    printf("\tJoined: %i\n\tPolicy: %i\n",detachstate,policy);
    printf("\tPriority: %i\n",priority);
    printf("\tAffinity:\n");
    for (int i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpuset))
            printf("\t\tCPU %d\n", i);

    // Change default attributes
    rc = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    rc = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = 99;	//highest priority
    rc = pthread_attr_setschedparam(&attr,&param);

    // Get new attributes values to be printed
    rc = pthread_attr_getdetachstate(&attr,&detachstate);
    rc = pthread_attr_getschedpolicy(&attr,&policy);
    rc = pthread_attr_getschedparam(&attr,&param);
    priority=param.sched_priority;

    CPU_ZERO(&cpuset);
    for (int i = 0; i < 8; i++) CPU_SET(i, &cpuset);
    rc = pthread_attr_setaffinity_np(&attr,sizeof(cpu_set_t), &cpuset);
    //if (s != 0) handle_error_en(s, "pthread_setaffinity_np");


    // Print custom attributes
    printf("Custom attributes:\n");
    printf("\tJoined: %i\n\tPolicy: %i\n",detachstate,policy);
    printf("\tPriority: %i\n",priority);
    printf("\tAffinity:\n");
    for (int i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpuset))
            printf("\t\tCPU %d\n", i);

    // put spaces in screen
    printf("\n");

    // Create threads
    for(t=0;t<NUM_THREADS;t++){
    	priority = 99-t;
    	param.sched_priority = priority; 	// down priority
    	rc = pthread_attr_setschedparam(&attr,&param);
    	rc = pthread_create(&threads[t], &attr, functiont, (void *)t);
    	if (rc){
    		printf("ERROR; function pthread_create() has returned code %d\n", rc);
    		exit(-1);
    	}
    	printf("main: thread %li created with priority = %i\n", t, priority);
    } // end of for sentences

    // Joining threads
    for(t=0;t<NUM_THREADS;t++){
    	rc = pthread_join(threads[t],&status);
    	if(rc){
    		printf("ERROR: function pthread_join() has returned code %d\n",rc);
    		exit(-1);
    	}
    	printf("main: thread #%ld joined with status %d\n",t,(long)status);
    }
    /* Last thing that main() should do */
    printf("main: program finished\n");
    pthread_exit(NULL);
}
