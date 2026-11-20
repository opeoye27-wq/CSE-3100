#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Exploring race condition problem

long count = 0; //2 threads are trying to increment this value simultaneously
long count1 = 0;

//if multiple things are trying to manipulate a variable then we must lock it with a mutex
pthread_mutex_t mtx; //define the mutex
pthread_mutex_t mtx1;

//if you have multiple things you want to protect, define a mutex for each of them

void *increase(void *arg)
{
    long increment = *(long *)arg; //since we're passing arg as void * we need to convert is as a long * (Can also do long *increment = (long *)arg instead)
    long local = 0; //let each thread have a local counter instead and then we'll lock the global counter and add our computation to it

    for (long i = 0; i < increment; i++){
        local ++; //each thread has 2 different locals and can increment it independently
    }
    pthread_mutex_lock(&mtx);
    count += local; 
    pthread_mutex_unlock(&mtx);  
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