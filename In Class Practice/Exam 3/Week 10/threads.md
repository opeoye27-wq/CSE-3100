# Threads

Creating a new process can be expensive because each process needs its own virtual address space. However, what if we want something more lightweight? That is where threads come in. Instead of having separate virtual address spaces, threads within the same process share one single virtual address space. This makes creating threads faster and less expensive than creating new processes.

## Process and Threads Comparison

| Feature | Processes | Threads |
|---|---|---|
| Virtual address space | Separate | Shared |
| Global variables | Separate copies after `fork()` | Shared |
| Heap | Separate | Shared |
| Stack | Each process has its own stack | Each thread has its own stack |
| Open file descriptors | Separate file descriptor tables, but can share open file entries | Shared file descriptor table |
| Communication | Inter-process communication, such as pipes | Shared memory |

> **Note:** Each thread also has its own registers, including the program counter and stack pointer.

## Why Use Threads?

- **Lightweight:** Creating a new thread requires fewer resources.
- **Easier communication:** Directly access shared memory.
- **Better CPU utilization:** When one thread blocks, another thread can be doing something else.
- **Concurrency:** Multiple threads can work on different tasks at the same time.
- **Parallelism:** On a multicore CPU, threads can run simultaneously, which can improve performance.

## Issues with Threads

Since threads of the same process share a single virtual address space, one thread can modify data another thread is reading. This is usually caused by race conditions. This would not have happened with processes because each process has its own virtual address space, so modifying one variable in one process does not affect the other.

## Concurrency and Parallelism

At any point in time, a single CPU core can ONLY run one thread of one process at a time.

### Concurrency: Single-Core CPU

- **How it works:** A single core runs only one instruction or thread at a time.
- **Illusion of multitasking:** The operating system uses a scheduler to switch back and forth between programs, processes, or threads. This is called **context switching**. It happens so quickly that it creates the illusion of parallelism, where many tasks are done at the same time, but the CPU is actually just quickly switching between tasks.

### Parallelism: Multi-Core CPU

- **How it works:** Each core in the CPU can run a separate program, process, or thread at a single point in time. This is true parallelism, though it does use more power.

## Pthread API

Below are some functions you use when writing programs with threads, but there are also others.

### Creating a Thread

```c
#include <pthread.h>

int pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg
);
```

| Function | Description | Parameters | Return |
|---|---|---|---|
| `pthread_create` | Creates a new thread. | `thread`: Stores the ID of the newly created thread.<br>`attr`: Thread attributes; use `NULL` for defaults.<br>`start_routine`: Function the new thread executes.<br>`arg`: Arguments passed to the thread function. | Success: `0`.<br>Failure: nonzero. |

### Understanding the Main Thread Function

Every thread starts by executing a function with this signature.

```c
void *thread_main(void *arg);
```

- `void *` as the parameter type allows us to pass in any type of argument.
- `void *` as the return type allows the function to return any type of result.
- `thread_main` is just the name of the function. You can name it anything you want.

> **Note:** This is basically the function you pass in as the `start_routine` argument for `pthread_create`.

### Joining a Thread

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

| Function | Description | Parameters | Return |
|---|---|---|---|
| `pthread_join` | Allows one thread to wait for another thread to finish execution and collect its resources. | `thread`: ID of the thread to wait for.<br>`retval`: Stores the thread's return value; use `NULL` if you do not need it. | Success: `0`.<br>Failure: nonzero. |

> **Note:** `pthread_join` is a blocking call. It will wait for the specified thread to terminate.

#### What Resources Are Reclaimed?

Any resources left that can be reclaimed will be reclaimed after calling `pthread_join`.

- Thread stack
- Thread control information
- Thread termination result

#### What Resources Are Not Reclaimed?

Since some of the resources listed below cannot be reclaimed automatically, you must manually clean them up when they are no longer needed.

- Global variables
- Heap allocations
- Process virtual address space
- Shared file descriptors

### What If Created Threads Are Not Joined?

| Situation | What Happens? |
|---|---|
| The main thread returns before the worker finishes. | If you return from the `main()` function, the process running the program is terminated, and therefore all the threads of the process are also terminated. |
| The worker threads finish, but the main thread continues to run without joining the terminated worker threads. | Since the program continues to run after the worker threads terminate, some resources remain allocated but unused. Eventually, you can run out of resources to create new threads. |

### Detaching a Thread

| Function | Description | Parameters | Return |
|---|---|---|---|
| `pthread_detach` | Makes an existing joinable thread detached so that its remaining resources are reclaimed when it terminates. | `thread`: ID of the existing joinable thread to detach. | Success: `0`.<br>Failure: nonzero. |

> **Note:** `pthread_detach` can also detach the calling thread.

### Terminating a Thread

```c
#include <pthread.h>

void pthread_exit(void *retval);

int pthread_cancel(pthread_t thread);
```

| Function | Description | Parameters | Return |
|---|---|---|---|
| `pthread_exit` | Terminates the calling thread without terminating the entire process. | `retval`: Pointer to the thread's termination result. This result can be collected by another thread that called `pthread_join` on this thread. Use `NULL` if the result is not needed. | None |
| `pthread_cancel` | Sends a request to terminate another thread. | `thread`: ID of the thread you want to request cancellation for. | Success: `0`, meaning the cancellation request was successfully sent. Failure: nonzero. |

> **Notes:**
>
> 1. This is different from `exit()`, which terminates the entire process and its threads.
> 2. Cancellation is a request. It does not mean the specified thread will terminate immediately. The thread to cancel must reach a cancellation point (`read`, `write`, `sleep`, etc.).

### Thread Identity

```c
#include <pthread.h>

pthread_t pthread_self(void);

int pthread_equal(pthread_t t1, pthread_t t2);
```

| Function | Description | Parameters | Return |
|---|---|---|---|
| `pthread_self` | Returns the ID of the calling thread. | None | ID of the calling thread |
| `pthread_equal` | Checks whether two thread IDs refer to the same thread. | `t1`: First thread ID.<br>`t2`: Second thread ID. | True: nonzero.<br>False: `0`. |

## Race Condition

A race condition happens when the behavior or result of a program depends on the timing or order in which multiple threads are executed. This is a very big problem, especially when multiple threads access shared data without synchronization.

### Example: Race Condition

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 2
#define MAX_INCREMENT 100000

// Define the structure for thread arguments.
typedef struct thread_data {
    int thread_num;
    int *sum;
} TData;

void *thread_main(void *targs) {
    // Cast the argument to the correct type.
    TData *thread_data = (TData *)targs;

    // Collect the thread arguments.
    int thread_num = thread_data->thread_num;
    int *sum = thread_data->sum;

    /*
        Increment by 1 to the shared variable "sum".

        However, sometimes you will notice that the result is not what you expected.
    */
    for (int i = 0; i < MAX_INCREMENT; i++) {
        (*sum)++;
    }

    // Terminate the thread.
    pthread_exit(NULL);
}

int main() {
    // Declare the array of thread IDs and thread data.
    pthread_t thread_ids[MAX_THREADS];
    TData thread_datas[MAX_THREADS];

    int sum = 0;

    // Initialize the thread arguments.
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_datas[i].thread_num = i;
        thread_datas[i].sum = &sum;
    }

    int status;

    // Create the threads.
    for (int i = 0; i < MAX_THREADS; i++) {
        status = pthread_create(
            &thread_ids[i],          // Pointer to thread ID
            NULL,                    // Pointer to attribute
            thread_main,             // Pointer to worker thread main function
            (void *)&thread_datas[i] // Pointer to worker thread data and arguments
        );

        // Always check if pthread_create failed.
        if (status != 0) {
            fprintf(stderr, "pthread_create failed with thread number: %d", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join the threads.
    for (int i = 0; i < MAX_THREADS; i++) {
        status = pthread_join(thread_ids[i], NULL);

        // Always check if pthread_join failed.
        if (status != 0) {
            fprintf(stderr, "pthread_join failed with thread number: %d", i);
            exit(EXIT_FAILURE);
        }
    }

    /*
        Print the final sum.

        In this case, because there are 2 threads, we should be expecting
        the result to be 200000. However, that is not always the case.

        Since the threads are accessing the shared variable "sum" without
        any protection, race conditions will occur.

        What exactly is happening?

        The operating system can switch between different threads.
        There is no specific order in which threads get run next; it depends
        on the scheduler.

        Imagine this:

        1. Thread 1 reads variable "sum" - It saves that value (let's say 0) in its register.
        2. Context switch happens - Passes the turn to Thread 2.
        3. Thread 2 reads variable "sum" - It saves that value (which is still 0) in its register.
        4. Thread 2 reads the next instruction to increment - Increments the value saved in its register by 1.
        5. Thread 2 writes the result to memory - The value of the variable "sum" should now be 1.
        6. Repeat steps 3-5 (50 times) - The value of the variable "sum" should now be 51.
        7. Context switch happens - Passes the turn to Thread 1.
        8. Thread 1 reads the next instruction to increment - Increments the value saved in its register (which is 0) by 1.
        9. Thread 1 writes the result to memory - The value of the variable "sum" should now be 1.

        Note: As long as threads are accessing the same memory location (the address),
        a race condition can happen unless you add protection.
    */

    fprintf(stdout, "Final Sum - Expected: %d - Got: %d\n",
            MAX_THREADS * MAX_INCREMENT, sum);

    return 0;
}
```

## Embarrassingly Parallel Problems

Some problems can be very easily split into smaller workloads to be distributed between multiple threads without much coordination or any shared part of the memory between the threads.

Examples: Matrix problems, 3D rendering, and others.

### 1. Chunking the Workload

You can split the work into about equal chunks and distribute it between threads.

```c
base_size = num_work / num_threads;

remainder = num_work % num_threads;

offset = thread_num * base_size + (thread_num < remainder ? thread_num : remainder);

count = base_size + (thread_num < remainder ? 1 : 0);
```

- `base_size` - The base size of work for each thread.
- `remainder` - The remaining work that cannot be evenly distributed.
- `offset` - The starting index of the work.
- `count` - The actual amount of work.

We will distribute the extra work to the first `remainder` threads. Basically, if there are 10 threads and the remainder is 5, we will distribute 1 extra work item to each of the first 5 threads. Threads 0-4 will have `base_size + 1` work items, and threads 5-9 will have `base_size` work items.

> **Note:** This works if all the work to distribute is equal. If it is not, you might have to use interleaving.

#### Example: Sum a Whole Array Using Chunking

Let's say we have the following integer array:

- `array_elements = 14`
- `num_threads = 4`

Now we calculate:

```c
base_size = array_elements / num_threads = 3;

remainder = array_elements % num_threads = 2;
```

The first two threads get 1 extra work item.

| Thread Number (`thread_num`) | Number of Work (`count`) | Calculation for Offset | Start Index | End Index (Exclusive) |
|---|---|---|---|---|
| 0 | 3 + 1 = 4 | 0 * 3 + 0 = 0 | 0 | 4 |
| 1 | 3 + 1 = 4 | 1 * 3 + 1 = 4 | 4 | 8 |
| 2 | 3 + 0 = 3 | 2 * 3 + 2 = 8 | 8 | 11 |
| 3 | 3 + 0 = 3 | 3 * 3 + 2 = 11 | 11 | 14 |

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 5
#define MAX_ARRAY_ELEMENTS 10003

typedef struct thread_data {
    int *arr;
    int thread_num;
    int start_index;
    int end_index;
    int result;
} TData;

void *sum_array(void *targs) {
    // Convert to correct type.
    TData *thread_data = (TData *)targs;

    // Grab the data you need from thread_data.
    int *arr = thread_data->arr;
    int start_index = thread_data->start_index;
    int end_index = thread_data->end_index;
    int result = thread_data->result;

    for (int i = start_index; i < end_index; i++) {
        result += arr[i];
    }

    // Save the result.
    thread_data->result = result;

    // Terminate the thread.
    pthread_exit(NULL);
}

int main() {
    // The array containing the integers we want to sum.
    int arr[MAX_ARRAY_ELEMENTS];

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        arr[i] = i;
    }

    // Declare the thread IDs and thread data.
    pthread_t thread_ids[MAX_THREADS];
    TData thread_datas[MAX_THREADS];

    // Initialize the thread data.
    int base_size = MAX_ARRAY_ELEMENTS / MAX_THREADS;
    int remainder = MAX_ARRAY_ELEMENTS % MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; i++) {
        // Do the calculations to split the work.
        int thread_num = i;

        int offset = thread_num * base_size +
                     (thread_num < remainder ? thread_num : remainder);

        int count = base_size + (thread_num < remainder ? 1 : 0);

        // Now we have the start and end index.
        int start_index = offset;
        int end_index = offset + count;

        thread_datas[i].thread_num = thread_num;
        thread_datas[i].arr = arr;
        thread_datas[i].start_index = start_index;
        thread_datas[i].end_index = end_index;
        thread_datas[i].result = 0;
    }

    // Create the threads.
    int status;

    for (int i = 0; i < MAX_THREADS; i++) {
        status = pthread_create(
            &thread_ids[i],
            NULL,
            sum_array,
            (void *)&thread_datas[i]
        );

        if (status != 0) {
            // Always check if pthread_create failed.
            fprintf(stderr, "pthread_create failed with thread number: %d", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join the threads.
    for (int i = 0; i < MAX_THREADS; i++) {
        status = pthread_join(thread_ids[i], NULL);

        if (status != 0) {
            fprintf(stderr, "pthread_join failed with thread number: %d", i);
            exit(EXIT_FAILURE);
        }
    }

    // Sum the result from each thread.
    int sum = 0;

    for (int i = 0; i < MAX_THREADS; i++) {
        sum += thread_datas[i].result;
    }

    fprintf(stdout, "Final Sum - Expected: %d - Got: %d\n",
            ((1 + 10002) * 10002) / 2, sum);

    return 0;
}
```

### 2. Interleaving the Workload

Instead of chunking, sometimes it might be better to interleave the workload. This means if there are `N` threads, then thread `X` will work on every `X + N` work item.

#### Example: Sum a Whole Array Using Interleaving

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 5
#define MAX_ARRAY_ELEMENTS 10003

typedef struct thread_data {
    int *arr;
    int thread_num;
    int arr_len;
    int result;
} TData;

void *sum_array(void *targs) {
    // Convert to correct type.
    TData *thread_data = (TData *)targs;

    // Grab the data you need from thread_data.
    int thread_num = thread_data->thread_num;
    int *arr = thread_data->arr;
    int arr_len = thread_data->arr_len;
    int result = thread_data->result;

    /*
        Instead of chunking the workload like before, we made each
        thread work on every thread_num + num_threads items.

        If thread_num = 0, then it will work on elements 0, 5, 10, 15, ...
        If thread_num = 1, then it will work on elements 1, 6, 11, 16, ...

        And so on ...
    */

    for (int i = thread_num; i < arr_len; i += MAX_THREADS) {
        result += arr[i];
    }

    // Save the result.
    thread_data->result = result;

    // Terminate the thread.
    pthread_exit(NULL);
}

int main() {
    // The array containing the integers we want to sum.
    int arr[MAX_ARRAY_ELEMENTS];

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        arr[i] = i;
    }

    // Declare the thread IDs and thread data.
    pthread_t thread_ids[MAX_THREADS];
    TData thread_datas[MAX_THREADS];

    // Initialize the thread data.
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_datas[i].thread_num = i;
        thread_datas[i].arr = arr;
        thread_datas[i].arr_len = MAX_ARRAY_ELEMENTS;
        thread_datas[i].result = 0;
    }

    // Create the threads.
    int status;

    for (int i = 0; i < MAX_THREADS; i++) {
        status = pthread_create(
            &thread_ids[i],
            NULL,
            sum_array,
            (void *)&thread_datas[i]
        );

        if (status != 0) {
            // Always check if pthread_create failed.
            fprintf(stderr, "pthread_create failed with thread number: %d", i);
            exit(EXIT_FAILURE);
        }
    }

    // Join the threads.
    for (int i = 0; i < MAX_THREADS; i++) {
        status = pthread_join(thread_ids[i], NULL);

        if (status != 0) {
            fprintf(stderr, "pthread_join failed with thread number: %d", i);
            exit(EXIT_FAILURE);
        }
    }

    // Sum the result from each thread.
    int sum = 0;

    for (int i = 0; i < MAX_THREADS; i++) {
        sum += thread_datas[i].result;
    }

    fprintf(stdout, "Final Sum - Expected: %d - Got: %d\n",
            ((1 + 10002) * 10002) / 2, sum);

    return 0;
}
```

## Important Takeaways

- Creating more threads does not always speed up computation. Creating new threads is not free, and they still need to be managed, which adds complexity. Only create the number of threads necessary to finish a job.
- When threads access the same part of memory and write to it, they can experience a race condition unless synchronization protection is in place.
- Some problems are **embarrassingly parallel**, meaning they can be easily split into multiple smaller tasks that each thread can handle without accessing a shared part of memory.