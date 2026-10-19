# Processes in C

# 1. Process Creation: `fork()`

## 1.1 Overview

To create a child process from a parent process, use the `fork()` function.

```c
#include <unistd.h>

pid_t fork(void);
```

**Purpose:** Creates a child process by duplicating the calling process.

### Return Values

| Return Value | Meaning |
|---|---|
| `-1` | Failed to create the child process. |
| `0` | This is the child process. |
| Greater than `0` | This is the parent process. The value is the child's PID. |

### Important Things to Remember

- Both parent and child continue executing from the point immediately after `fork()`.
- The child does not restart `main()`.
- The operating system scheduler determines which process executes first.
- Each process has its own Process ID (PID).
- Always check whether `fork()` succeeded.

## Example: Basic Parent and Child Process

```c
// process_fork.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed.
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Hello from Child.\n");
        _exit(0);
    } else {
        // Parent process
        printf("Hello from Parent.\n");
        wait(NULL);
    }

    return 0;
}
```

**Possible output:**

```text
Hello from Parent.
Hello from Child.
```

The child might print first instead. The order is not guaranteed.

**Important:** `_exit()` terminates the child immediately without flushing standard I/O buffers. If you use `printf()` immediately before `_exit()`, flush `stdout` first or use another appropriate termination approach to ensure the output is delivered.

---

# 2. What Does `fork()` Copy?

When `fork()` creates a child process, the child receives a logical copy of the parent's execution state and memory.

| Component | What Happens? |
|---|---|
| Program code | Parent and child can share physical code pages because the code is typically read-only. |
| Stack / local variables | Each process has its own logical copy. |
| Heap / dynamically allocated memory | Each process has its own logical copy. |
| Global / static variables | Each process has its own logical copy. |
| Execution state | Both resume immediately after the `fork()` call. |
| Output buffers | Unflushed user-space buffers are copied. |
| File descriptors | The child receives copies of the parent's file descriptor entries. The corresponding open file descriptions are shared. |
| Process ID (PID) | The child receives a new PID. |

## 2.1 Copy-on-Write (COW)

Memory is not necessarily copied immediately when `fork()` is called. Instead, operating systems commonly use **Copy-on-Write (COW)**.

How it works:

1. Initially, the parent and child can share the same physical memory pages.
2. If either process attempts to modify a shared page, the operating system creates a private copy for that process.
3. Changes to ordinary private variables in one process do not affect the other process.

This makes creating a child process more efficient because the operating system avoids copying all memory immediately.

## Example: Independent Memory

```c
// process_independent_mem.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int x = 10;
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process
        x += 10;
        printf("Child Process - x value is: %d\n", x);
        _exit(0);
    } else {
        // Parent process
        x += 20;
        printf("Parent Process - x value is: %d\n", x);
        wait(NULL);
    }

    return 0;
}
```

**Expected values:**

- Child: `x = 20`
- Parent: `x = 30`

Each process modifies its own logical copy of `x`.

> **Note:** Because the child uses `_exit()`, its `printf()` output may not appear unless the output is flushed first. The values above describe the intended results.

---

# 3. Output Buffering and `fork()`

## 3.1 What Is Output Buffering?

Standard I/O functions, such as `printf()`, may store output in a user-space buffer before sending it to the operating system.

The output buffer may be flushed when:

- The buffer fills up.
- `fflush()` is called.
- The stream is closed.
- The program terminates normally.

The exact buffering behavior depends on the stream and environment.

**Important:** Unflushed user-space output buffers are copied when `fork()` is called.

This means both processes may later flush the same buffered text, causing duplicate output.

## Example: Buffering Before `fork()`

```c
// process_output_buf.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("Hello");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        _exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}
```

If `"Hello"` remains in the output buffer when `fork()` happens, the parent and child each inherit a copy of it.

If both processes flush their buffers, the output may become:

```text
HelloHello
```

However, in this particular example, the child uses `_exit()`, which does not flush the inherited buffer. Therefore, the child does not flush its copy, and duplicate output is not expected from the child.

## 3.2 Solution: Flush Before `fork()`

Call `fflush(stdout)` before forking to ensure pending standard output has been flushed.

```c
// process_output_buf.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("Hello");
    fflush(stdout);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        _exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}
```

Now the pending `"Hello"` is flushed before `fork()`, so the child does not inherit a pending copy of that output.

---

# 4. Process Identification

The operating system provides functions for retrieving process IDs.

```c
#include <unistd.h>

pid_t getpid(void);   // Returns the current process's PID.
pid_t getppid(void);  // Returns the current process's parent's PID.
```

| Function | Purpose |
|---|---|
| `getpid()` | Returns the PID of the calling process. |
| `getppid()` | Returns the PID of the calling process's parent. |

## Example: Parent and Child PIDs

```c
// process_id.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process
        pid_t my_pid = getpid();
        pid_t parent_pid = getppid();

        printf("Child Process - My PID is %d - "
               "My Parent's PID is %d\n",
               (int)my_pid, (int)parent_pid);

        fflush(stdout);
        _exit(0);
    } else {
        // Parent process
        pid_t my_pid = getpid();

        printf("Parent Process - My PID is %d - "
               "My Child's PID is %d\n",
               (int)my_pid, (int)pid);

        wait(NULL);
    }

    return 0;
}
```

The parent knows the child's PID from the return value of `fork()`. The child can obtain its own PID using `getpid()` and its parent's PID using `getppid()`.

---

# 5. Waiting for Children: `wait()`

## 5.1 Overview

The `wait()` function allows a parent process to wait for one of its child processes to terminate and collect its termination status.

```c
#include <sys/wait.h>

pid_t wait(int *status);
```

### Parameters

| Parameter | Meaning |
|---|---|
| `int *status` | Pointer to an integer where the child's termination information is stored. Pass `NULL` if you do not need that information. |

### Return Values

| Return Value | Meaning |
|---|---|
| Greater than `0` | PID of the child that was reaped. |
| `-1` | An error occurred, or no eligible child exists. |

### Important Things to Remember

- `wait()` blocks until a child terminates, if no terminated child is already available to collect.
- It collects the child's termination status and reaps the child.
- Reaping prevents the terminated child from remaining as a zombie process.
- `wait(NULL)` waits for a child without collecting its status into a variable.

## Example: Using `wait()`

```c
// process_wait.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Perform some work.
        _exit(0);
    } else {
        // Parent process
        // Perform some work.
        wait(NULL);
    }

    return 0;
}
```

The parent waits for the child to finish before continuing past `wait(NULL)`.

---

# 6. Waiting for Specific Children: `waitpid()`

## 6.1 Overview

Use `waitpid()` when you want to wait for a specific child or control how the parent checks the child's state.

```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

### The `pid` Argument

| Value | Behavior |
|---|---|
| Greater than `0` | Wait for the child with that specific PID. |
| `-1` | Wait for any child. |

### Important Options

| Option | Meaning |
|---|---|
| `0` | Default behavior: block until an eligible child state change occurs, normally termination. |
| `WNOHANG` | Return immediately if no eligible child state change is available. |
| `WUNTRACED` | Also report children that have stopped and have not been reported yet. |
| `WCONTINUED` | Also report children that have resumed after being stopped. |

You can combine options using the bitwise OR operator `|`.

```c
waitpid(pid, &status, WNOHANG | WUNTRACED);
```

### Return Values

| Return Value | Meaning |
|---|---|
| Greater than `0` | PID of the child whose state change was reported. |
| `0` | `WNOHANG` was specified and no reportable state change was available. |
| `-1` | An error occurred. |

**Important:** A child is reaped when `waitpid()` reports its termination. If `WNOHANG` returns `0`, the child has not been reaped and may need to be checked again later. Similarly, reporting a stopped or continued child does not reap it.

## Example: Using `waitpid()`

```c
// process_waitpid.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process
        _exit(0);
    } else {
        // Parent process
        int status;

        // Wait for this specific child.
        pid_t result = waitpid(pid, &status, 0);

        if (result == -1) {
            perror("waitpid()");
            exit(1);
        }

        if (WIFEXITED(status)) {
            printf("Exit Code: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Killed by signal: %d\n", WTERMSIG(status));
        }
    }

    return 0;
}
```

This example waits for the child with PID `pid` and checks whether it exited normally or was terminated by a signal.

---

# 7. Interpreting Child Exit Status

The `status` value returned through `wait()` or `waitpid()` contains encoded information about how the child changed state.

Use the macros in `<sys/wait.h>` to interpret it.

### Status Macros

| Macro | Purpose |
|---|---|
| `WIFEXITED(status)` | Returns true if the child terminated normally. |
| `WEXITSTATUS(status)` | Gets the child's exit code after normal termination. Only use when `WIFEXITED(status)` is true. |
| `WIFSIGNALED(status)` | Returns true if the child was terminated by a signal. |
| `WTERMSIG(status)` | Gets the signal number that terminated the child. Only use when `WIFSIGNALED(status)` is true. |
| `WIFSTOPPED(status)` | Returns true if the child was stopped by a signal. |
| `WSTOPSIG(status)` | Gets the signal number that stopped the child. Only use when `WIFSTOPPED(status)` is true. |
| `WIFCONTINUED(status)` | Returns true if the child resumed after being stopped. |

### Example: Checking the Exit Code

```c
int status;
pid_t result = waitpid(pid, &status, 0);

if (result == -1) {
    perror("waitpid()");
} else if (WIFEXITED(status)) {
    printf("Exit Code: %d\n", WEXITSTATUS(status));
} else if (WIFSIGNALED(status)) {
    printf("Killed by signal: %d\n", WTERMSIG(status));
}
```

### Understanding the Difference

- **Normal termination:** The child called `exit()`, returned from `main()`, or used a related normal termination mechanism.
- **Signal termination:** The child was terminated by a signal, such as `SIGKILL`.
- **Stopped:** The child was paused by a signal but has not terminated.
- **Continued:** A previously stopped child resumed execution.

Always check the appropriate `WIF...` macro before using its corresponding status-extraction macro.

---

# 8. Error Codes from `wait()` and `waitpid()`

When a wait function fails, it returns `-1` and sets `errno` to indicate the error.

Include the appropriate headers:

```c
#include <errno.h>
#include <sys/wait.h>
```

### Common Errors

| Error | Meaning |
|---|---|
| `ECHILD` | There are no eligible child processes to wait for, or the specified PID is not a waitable child of the calling process. |
| `EINTR` | The wait operation was interrupted by a signal before a reportable child state change was returned. |
| `EINVAL` | An invalid option was passed to `waitpid()`. |

### 8.1 `ECHILD`

This can happen when:

- The calling process has no children left to wait for.
- All relevant children have already been reaped.
- The specified PID is not a child process that the caller can wait for.

### 8.2 `EINTR`

This happens when a signal interrupts the wait operation.

Depending on the program, you may need to retry the wait operation if it returns `-1` and `errno == EINTR`.

### 8.3 `EINVAL`

This occurs when `waitpid()` receives an invalid option.

---

# 9. Zombie and Orphan Processes

## 9.1 Zombie Processes

A **zombie process** is a child process that has terminated, but whose parent has not yet collected its termination status.

After the child terminates, most of its resources are released. However, a small entry remains in the process table so the parent can retrieve the child's exit status.

Why zombies matter:

- The terminated process remains represented in the process table.
- Too many unreaped children can consume process-table resources.
- Process IDs are limited, so excessive unreaped processes can contribute to resource exhaustion.

### How to Prevent Zombies

Use `wait()` or `waitpid()` to collect terminated children's statuses.

```c
wait(NULL);
```

Or, for a specific child:

```c
int status;
waitpid(pid, &status, 0);
```

## 9.2 Orphan Processes

An **orphan process** is a child process that continues running after its parent terminates.

The operating system reparents the orphan to an appropriate system process, commonly an init or service-management process, which can eventually collect its termination status.

### Zombie vs. Orphan

| Feature | Zombie Process | Orphan Process |
|---|---|---|
| Child's state | Has terminated | Still running |
| Parent | Has not yet collected the child's termination status | Has terminated |
| Main issue | Remains in the process table until reaped | Needs a new parent |
| What happens next? | A parent or appropriate process must reap it | The operating system reparents it |

**Remember:**

- **Zombie:** The child is dead, but its termination status has not been collected.
- **Orphan:** The child is alive, but its parent has terminated.

---

# 10. Quick Review: Important Functions

| Function | Purpose |
|---|---|
| `fork()` | Creates a child process. |
| `getpid()` | Gets the current process's PID. |
| `getppid()` | Gets the current process's parent PID. |
| `wait()` | Waits for and reaps a terminated child. |
| `waitpid()` | Waits for a selected child or child state change. |
| `_exit()` | Terminates a process immediately without flushing standard I/O buffers. |
| `exit()` | Terminates a process after normal C library cleanup, including flushing standard I/O streams. |

## The Big Picture

1. Use `fork()` to create a child process.
2. Check the return value to determine whether you're in the parent or child.
3. Remember that parent and child have independent logical memory, commonly implemented using Copy-on-Write.
4. Be aware that unflushed output buffers are copied during `fork()`.
5. Use `getpid()` and `getppid()` to identify processes.
6. Use `wait()` or `waitpid()` to collect child termination statuses.
7. Use the `WIF...` and `W...STATUS` macros to interpret those statuses.
8. Reap terminated children to prevent zombie processes.

