#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
    unsigned int start, end; //so each thread can know what rows its responsible for
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void * p_arg)
{
    // TODO
    //get this thread's arguments
    thread_arg_t *my_data = (thread_arg_t *) p_arg;

    //loop through and process thread's assigned rows
    for (unsigned int i = my_data->start; i < my_data->end; i ++){
        for (unsigned int j = 0; j < my_data->m->ncols; j++){
            my_data->t->data[i][j] = my_data->m->data[i][j] + my_data->n->data[i][j]; //process every column in assigned row
        }
    }
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    // TODO
    pthread_t threads[NUM_THREADS];
    thread_arg_t thread_args[NUM_THREADS];

    //creating the 2 threads
    for (unsigned int i = 0; i < NUM_THREADS; i ++)
    {
        thread_args[i].id = i;
        thread_args[i].start = i * m->nrows / NUM_THREADS;
        thread_args[i].end = (i + 1) * m->nrows / NUM_THREADS;

        thread_args[i].m = m;
        thread_args[i].n = n;
        thread_args[i].t = t;

        int rc = pthread_create(&threads[i], NULL, thread_main, &thread_args[i]);

        if (rc != 0){
            fprintf(stderr, "pthread_create failed: %d\n", rc);
            //wait for threads already created
             for (unsigned int j = 0; j < i; j++){
                pthread_join(threads[j], NULL);
             }
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }
    // Wait for both threads to finish
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_join failed: %d\n", rc);
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }
    return t;
}