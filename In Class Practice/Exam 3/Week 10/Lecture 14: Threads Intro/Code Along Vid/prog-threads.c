#include <stdio.h>
#include <unistd.h>
#include <pthread.h> //pthread API
#include <stdlib.h>

void *task(void *arg){ //function must follow this syntax of void*
    //FIRST: get void* arg as an integer so we can use it
    int *n = (int *)arg;

    int *result = calloc(1, sizeof(int)); //since we're returning result and its a local variable (will be lost once prog ends) we should store it on the heap
    for (int i = 1; i <= *n; i++)
    {
        *result += i;
        sleep(i); //ex i = 2: sleep for 3 seconds, i = 10: sleep for 55 seconds (for each iteration it will sleep for ith seconds)
    }
   //printf("result for n=%d is %d\n", *n, result);
    //each thread will run this function until it reads this line and then exits that thread only
    //can return using pthread_exit (DO NOT RETURN LOCAL VARIABLES i.e. result)
    pthread_exit(result); //takes addresses
}

int main(){
    //Step 1: initialize pthread identifiers
    pthread_t t1, t2; //makes 2 threads that will both run task function
    
    int v1 = 5;
    int v2 = 6;

    //step 2: create threads (worker threads)
    //REMEMBER: every time a process is created there's already one thread (main thread)
    pthread_create(&t1, NULL, task, &v1); //task(5)
    pthread_create(&t2, NULL, task, &v2); //task(6)

    //Step 3: join the threads (main threads must wait for worker threads)
    int *r1, *r2; //stores the returned NULL

    pthread_join(t1, (void **) &r1);
    printf("result for v=%d is %d\n", v1, *r1);
    
    pthread_join(t2, (void **) &r2);
    printf("result for v=%d is %d\n", v2, *r2);

    //Step 4: Terminate the main process
    return 0;
}