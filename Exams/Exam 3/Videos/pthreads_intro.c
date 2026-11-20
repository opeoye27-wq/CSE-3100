#include <stdio.h>
//https://www.youtube.com/watch?v=ldJ8WGZVXZk
#include <pthread.h> //access pthread library

void *computation(void *add)
{
    long *add_num = (long *) (add);//typecast the pointer then dereference it 
    printf("Add: %ld\n", *add_num);
    return NULL;
}

int main()
{
    pthread_t thread1; //use to create and manage our thread

    long value1 = 1;

    //pthread_create(&thread1, NULL, computation, NULL); //passing a pointer to thread1 variable, attribute to variable, function, pass parguments to function
    pthread_create(&thread1, NULL, computation, (void*) &value1);// can instead pass in a (void*) pointer to the variable and then the function 
    pthread_join(thread1, NULL); //join back to main thread (joinig thread, possible returned vale)
    return 0;
}

