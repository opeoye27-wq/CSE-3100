#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t; //m: first input matrix, n: second input matrix, t: output matrix that stores calculations
} thread_arg_t;

static void * thread_main(void * p_arg) //main function that threads will run
{
    // TODO
    thread_arg_t *my_data = (thread_arg_t *) p_arg;

    unsigned int mid = (my_data->m->nrows + 1) / 2; //find the midpoint
    unsigned int start;
    unsigned int end;

    if (my_data->id == 0) {
        start = 0;
        end = mid;
    }
    else {
        start = mid;
        end = my_data->m->nrows;
    }
    
    //matrix multiplication functionality from matrix.c but changing outermost for loop to calculate onyl assigned rows
    for (unsigned int i = start; i < end; i++)  {
        for (unsigned int j = 0; j < my_data->n->ncols; j++) {
            TElement sum = (TElement)0;
            for (unsigned int k = 0; k < my_data->m->ncols; k++)
                sum += my_data->m->data[i][k] * my_data->n->data[k][j];
            my_data->t->data[i][j] = sum;
        }
    }
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    // TODO
    //making the threads do the work
    pthread_t threads[NUM_THREADS]; //declare thread IDs
    thread_arg_t thread_data_array[NUM_THREADS]; //arguments
    int rc;
    for (int i = 0; i < NUM_THREADS; i ++)
    {
        thread_data_array[i].id = i; //thread id
        thread_data_array[i].m = m;
        thread_data_array[i].n = n;
        thread_data_array[i].t = t;
        rc = pthread_create(&threads[i], NULL, thread_main, &thread_data_array[i]);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    //main thread
    for (int i = 0; i < NUM_THREADS; i ++)
    {
        rc = pthread_join(threads[i], NULL);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    return t;
}
