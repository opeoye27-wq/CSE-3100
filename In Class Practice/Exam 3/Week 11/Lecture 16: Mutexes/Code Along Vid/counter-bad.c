#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Exploring race condition problem

long count = 0; //2 threads are trying to increment this value simultaneously

//if multiple things are trying to manipulate a variable then we must lock it with a mutex
pthread_mutex_t mtx; //define the mutex

void *increase(void *arg)
{
    long increment = *(long *)arg; //since we're passing arg as void * we need to convert is as a long * (Can also do long *increment = (long *)arg instead)
    
    //parallel
    for (long i = 0; i < increment; i++){
        //before doing any operations lock the mutex (specifically locking count++)
        pthread_mutex_lock(&mtx);
        count++;
        //unlock after operations
        pthread_mutex_unlock(&mtx);
    }
    //we're locking and unlocking the mutex multiple times which uses a lot of CPUs and takes a long time
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mtx, NULL);//must always initialize the mutex
    long increment = atol(argv[1]); //command line val that tells us how many times to increment; atol converts string to long

    //Create 2 threads
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increase, &increment);
    pthread_create(&t2, NULL, increase, &increment);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("counter is %ld\n", count);
    pthread_mutex_destroy(&mtx); //also remeber to destroy mutex
    return 0;
}