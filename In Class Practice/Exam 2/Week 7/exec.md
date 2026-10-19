# Exec Family Functions in C

## 1. Running a Program in a Forked Process

Sometimes, you want a child process to run a separate program. This is possible with the **`exec` family of functions**.

When you successfully call an `exec` function, it replaces the calling process's program image with a new one. The process keeps its **PID** and other attributes, such as open file descriptors.

Essentially, the process's memory layout is replaced with the new program's memory layout, including its:

- Code segment
- Stack
- Heap
- Global and static variables

> **Note:** Calling an `exec` function successfully does **not** create a new process. It transforms the existing calling process.

---

## 2. Exec Functions

### Function Prototypes

Include the `unistd.h` header file to use the `exec` functions.

```c
#include <unistd.h>

int execl(const char *path, const char *arg0, ... /* NULL */);
int execlp(const char *file, const char *arg0, ... /* NULL */);

int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
```

### Understanding the Function Names

The suffixes tell you how the functions handle arguments and executable paths.

| Suffix | Meaning | Description |
|---|---|---|
| `l` | List | Arguments are passed as a comma-separated list of individual strings. The list must end with `NULL`. |
| `v` | Vector | Arguments are passed as an array of string pointers. The array must end with `NULL`. |
| `p` | Path | The function searches the directories listed in the `PATH` environment variable for the executable. |

### Understanding `PATH`

When using `execlp()` or `execvp()`, you can provide an executable's filename instead of its full path.

To see the directories your system searches, run this command in the terminal:

```bash
echo $PATH
```

**Important:** If the executable name passed to `execlp()` or `execvp()` contains a `/`, the function treats it as a relative or absolute path instead of searching the directories in `PATH`.

### Quick Comparison

| Function | Arguments | Searches `PATH`? |
|---|---|---|
| `execl()` | Individual arguments | No |
| `execlp()` | Individual arguments | Yes |
| `execv()` | Argument array | No |
| `execvp()` | Argument array | Yes |

---

## 3. Examples

All four examples use `fork()` to create a child process. The child then calls an `exec` function to run the `echo` program.

The output in each case is:

```text
Hello World
```

### Example 1: Calling `execl()`

`execl()` takes the executable path followed by individual arguments. The argument list must end with `NULL`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process

        // First parameter: absolute path to the executable.
        // Remaining parameters: individual arguments, ending with NULL.
        execl("/bin/echo", "/bin/echo", "Hello World", NULL);

        // Only executes if exec fails.
        perror("exec()");
        _exit(1);
    } else {
        // Parent process
        // Wait for the child to finish.
        wait(NULL);
    }

    return 0;
}
```

**Key points:**

- `"/bin/echo"` specifies the executable's path.
- The second `"/bin/echo"` becomes `argv[0]`.
- `"Hello World"` becomes `argv[1]`.
- `NULL` marks the end of the argument list.

### Example 2: Calling `execlp()`

`execlp()` works like `execl()`, except it searches `PATH` for the executable when the filename contains no `/`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process

        // Searches PATH for the echo executable.
        execlp("echo", "echo", "Hello World", NULL);

        // Only executes if exec fails.
        perror("exec()");
        _exit(1);
    } else {
        // Parent process
        wait(NULL);
    }

    return 0;
}
```

**Key difference:** Instead of providing `"/bin/echo"`, we provide `"echo"` and let the function search `PATH`.

### Example 3: Calling `execv()`

`execv()` takes an argument array instead of a list of individual arguments.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process

        // Create the argument array.
        // The array must end with NULL.
        char *args[] = {
            "/bin/echo",
            "Hello World",
            NULL
        };

        // First parameter: executable path.
        // Second parameter: argument array.
        execv("/bin/echo", args);

        // Only executes if exec fails.
        perror("exec()");
        _exit(1);
    } else {
        // Parent process
        wait(NULL);
    }

    return 0;
}
```

**Key points:**

- `args` is an array of strings.
- `args[0]` is the program name.
- `args[1]` is `"Hello World"`.
- `NULL` marks the end of the argument array.

### Example 4: Calling `execvp()`

`execvp()` combines the features of `execv()` and `execlp()`: it accepts an argument array and searches `PATH` for the executable when the filename contains no `/`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {
        // Child process

        // Create the argument array.
        // The array must end with NULL.
        char *args[] = {
            "echo",
            "Hello World",
            NULL
        };

        // Searches PATH for the echo executable.
        execvp("echo", args);

        // Only executes if exec fails.
        perror("exec()");
        _exit(1);
    } else {
        // Parent process
        wait(NULL);
    }

    return 0;
}
```

**Key difference:** The executable is specified as `"echo"` rather than `"/bin/echo"`, so `execvp()` searches `PATH`.

---

## 4. Important Things to Remember

### 1. `exec()` Does Not Create a New Process

- `fork()` creates a new process.
- `exec()` replaces the calling process's program image.
- When a child calls `exec()`, it becomes the new program while remaining the same process.

### 2. Always Handle `exec()` Failure

If an `exec` function succeeds, it never returns to the original code.

If it fails, it returns `-1`, and the next line executes.

```c
execlp("echo", "echo", "Hello World", NULL);

// Only reached if exec fails.
perror("exec()");
_exit(1);
```

Using `_exit(1)` terminates the child process if execution fails.

### 3. Always Terminate Arguments with `NULL`

Both argument lists and argument arrays must end with `NULL`.

**List example:**

```c
execlp("echo", "echo", "Hello World", NULL);
```

**Array example:**

```c
char *args[] = {"echo", "Hello World", NULL};
execvp("echo", args);
```

Forgetting `NULL` can cause incorrect behavior.

### 4. The PID Is Preserved

When a child process calls `exec()`, its PID does not change.

The new program runs inside the same process that called `exec()`.

### 5. File Descriptors Are Preserved

Open file descriptors generally remain open across `exec()`, unless they have the close-on-exec flag set.

This means the new program can continue using inherited open files, pipes, and other file descriptors.

### 6. The Parent Should Wait for the Child

When using `fork()`, the parent can call `wait()` to wait for its child to finish.

```c
wait(NULL);
```

This helps the parent coordinate with the child and reap its terminated process.

---

## 5. Summary: Which Function Should I Use?

| Situation | Function |
|---|---|
| Individual arguments + explicit executable path | `execl()` |
| Individual arguments + search `PATH` | `execlp()` |
| Argument array + explicit executable path | `execv()` |
| Argument array + search `PATH` | `execvp()` |

**The easiest way to remember it:**

- **`l` = List** of individual arguments.
- **`v` = Vector** (an argument array).
- **`p` = Path search** using the `PATH` environment variable.
- **`exec` = Replace the current program image**, not create a process.

