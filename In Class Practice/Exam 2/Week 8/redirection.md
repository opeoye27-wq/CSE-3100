# Redirection

## Overview

Programs normally use three standard file descriptors:

| File descriptor | Name | Purpose |
|---:|---|---|
| `0` | `stdin` | Receives input, usually from the terminal |
| `1` | `stdout` | Sends normal output, usually to the terminal |
| `2` | `stderr` | Sends error messages, usually to the terminal |

Redirection changes which files these descriptors refer to, so a program can read input from a file or send output and errors to files instead of the terminal.

## 1. Shell Redirection Commands

| Command | Description |
|---|---|
| `cmd < infile` | Redirect standard input (`0`) to read from `infile`. |
| `cmd > outfile` | Redirect standard output (`1`) to `outfile`, truncating existing content. |
| `cmd >> outfile` | Append standard output (`1`) to `outfile`. |
| `cmd 1> outfile` | Explicitly redirect standard output (`1`), truncating the file. |
| `cmd 1>> outfile` | Explicitly append standard output (`1`) to the file. |
| `cmd 2> outfile` | Redirect standard error (`2`) to `outfile`, truncating the file. |
| `cmd 2>> outfile` | Append standard error (`2`) to the file. |
| `cmd &> outfile` | Redirect both standard output and standard error to `outfile` (in shells that support this syntax). |

**Important:** The file used for input must already exist. Output files are created if needed; `>` truncates an existing file, while `>>` appends.

### Examples

Run these commands in order:

1. Print to the terminal:
   ```sh
   echo "Hello World"
   ```

2. Send output to `hello.txt` instead of the terminal:
   ```sh
   echo "Hello World" > hello.txt
   ```
   The file is created if it does not exist. If it already exists, its previous contents are truncated.

3. Read input from `hello.txt`:
   ```sh
   grep -i "hello" < hello.txt
   ```
   `grep -i` finds lines matching `hello`, ignoring case. The matching lines appear in the terminal.

4. Read from `hello.txt` and save matching lines to `matches.txt`:
   ```sh
   grep -i "hello" < hello.txt > matches.txt
   ```

5. Send a program's error output to `error.txt`:
   ```sh
   grep -i "hello" nonexistent.txt > matches.txt 2> error.txt
   ```
   Here, `grep` runs but reports an error because `nonexistent.txt` was supplied as an input filename. Its error output is redirected to `error.txt`.

### Shell Errors vs. Program Errors

Redirection is processed by the shell before the command runs.

- **Shell error:** If a redirection itself fails, such as `< nonexistent.txt`, the shell reports the error and does not run the command.
- **Program error:** If redirection succeeds and the program runs, errors produced by the program can be written to the redirected standard error stream.

For example:

```sh
grep -i "hello" < nonexistent.txt > matches.txt 2> error.txt
```

The shell cannot open the input file, so `grep` does not run. The error is a shell redirection error, not a `grep` error, so `error.txt` will not contain a program-generated error message from `grep`.

## 2. File Descriptor Table, Open File Table, and Inode Table

Each process has its own **File Descriptor Table (FDT)**. Each file descriptor entry points to an entry in the **Open File Table (OFT)**, which in turn references file metadata represented by an inode.

| Feature | File Descriptor Table (FDT) | Open File Table (OFT) | Inode Table |
|---|---|---|---|
| Scope | Per process | System-wide kernel data structure | Filesystem |
| Purpose | Tracks a process's open file descriptors | Tracks open file descriptions and their current state | Tracks filesystem objects and their metadata |
| Contains | Entries referencing open file descriptions | File offset, status flags/access mode, and inode reference | Metadata such as permissions, size, and file location |
| Sharing | Each process has its own table | Multiple file descriptors can reference the same entry | Multiple openings can reference the same underlying file/inode |

**Relationship:**

```text
[File Descriptor Table] -> [Open File Table] -> [Inode]
```

- **FDT:** A per-process table mapping file descriptor numbers to open file descriptions.
- **OFT:** Tracks the state of an open file description, including the current file offset and access status.
- **Inode:** Represents a filesystem object and stores metadata about it. A file can persist after a process exits if it was created successfully.

## 3. Redirection with `dup()` and `dup2()`

```c
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
```

| Function | Description | Return value |
|---|---|---|
| `dup(oldfd)` | Creates a new descriptor using the lowest-numbered available descriptor. The new descriptor refers to the same open file description as `oldfd`. | New descriptor (`>= 0`) on success; `-1` on failure, with `errno` set. |
| `dup2(oldfd, newfd)` | Makes `newfd` refer to the same open file description as `oldfd`. If `newfd` is already open, it is closed first (unless `oldfd == newfd`). | `newfd` on success; `-1` on failure, with `errno` set. |

### Example: `dup()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    // Open a file for reading and writing, create if not exist, with permission 0644.
    int fd1 = open("file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd1 < 0) {
        // Always check if open() failed.
        perror("open");
        exit(EXIT_FAILURE);
    }

    /*
        This will assign a new lowest available file descriptor to reference
        the same Open File Table entry as the original old file descriptor.

        This means fd2 will point to the same Open File Table entry as fd1.
    */
    int fd2 = dup(fd1);
    if (fd2 < 0) {
        // Always check if dup() failed.
        perror("dup");
        exit(EXIT_FAILURE);
    }
    // Print the fd values
    fprintf(stdout, "fd1: %d\nfd2: %d\n", fd1, fd2);

    // !!!!! Write Message with fd1
    char write_message[] = "Hello World!";
    ssize_t bytes_written = write(fd1, write_message, sizeof(write_message));

    // Check the number of bytes written.
    if (bytes_written >= 0) {
        fprintf(stdout, "Wrote to fd1(%d): %.*s\n", fd1, (int)bytes_written, write_message);
    } else {
        // Check if write failed.
        perror("write");
    }

    // Go to beginning of file for reading.
    lseek(fd1, 0, SEEK_SET);

    // !!!!! Read Message with fd2
    char read_message[1024];
    ssize_t bytes_read = read(fd2, read_message, 1023);

    // Check the number of bytes read.
    if (bytes_read >= 0) {
        read_message[bytes_read] = '\0';
        fprintf(stdout, "Read from fd2(%d): %s\n", fd2, read_message);
    } else {
        // Check if read failed.
        perror("read");
    }

    // Please do not forget to close the open file descriptors when you are done.
    close(fd1);
    close(fd2);

    return 0;
}
```

**Key point:** `fd2` was created with `dup(fd1)`, so both descriptors refer to the same open file description. They share the file offset and status flags. After writing, the example uses `lseek(fd1, 0, SEEK_SET)` to move the shared offset back to the beginning before reading through `fd2`.

### Example: `dup2()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    // Open a file for reading and writing, create if not exist, with permission 0644.
    int fd1 = open("file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd1 < 0) {
        // Always check if open() failed.
        perror("open");
        exit(EXIT_FAILURE);
    }

    /*
        This will assign the specified file descriptor to reference the
        same Open File Table entry as the original old file descriptor.

        This means fd2 in this case `5` will point to the same Open File Table entry as fd1.
    */
    int fd2 = dup2(fd1, 5); // I chose 5 as the newfd. You can choose whatever you need.
    if (fd2 < 0) {
        // Always check if dup2() failed.
        perror("dup");
        exit(EXIT_FAILURE);
    }
    // The fd values
    fprintf(stdout, "fd1: %d\nfd2: %d\n", fd1, fd2);

    // !!!!! Write Message with fd1
    char write_message[] = "Hello World!";
    ssize_t bytes_written = write(fd1, write_message, sizeof(write_message));

    // Check the number of bytes written.
    if (bytes_written >= 0) {
        fprintf(stdout, "Wrote to fd1(%d): %.*s\n", fd1, (int)bytes_written, write_message);
    } else {
        // Check if write failed.
        perror("write");
    }

    // Go to beginning of file for reading.
    lseek(fd1, 0, SEEK_SET);

    // !!!!! Read Message with fd2
    char read_message[1024];
    ssize_t bytes_read = read(fd2, read_message, 1023);

    // Check the number of bytes read.
    if (bytes_read >= 0) {
        read_message[bytes_read] = '\0';
        fprintf(stdout, "Read from fd2(%d): %s\n", fd1, read_message);
    } else {
        // Check if read failed.
        perror("read");
    }

    // Please do not forget to close the open file descriptors when you are done.
    close(fd1);
    close(fd2);

    return 0;
}
```

**Key point:** `dup2(fd1, 5)` makes descriptor `5` refer to the same open file description as `fd1`. Unlike `dup()`, `dup2()` lets you specify the descriptor number.

## 4. Redirection with `fork()`, `exec()`, and `dup2()`

Shell redirection can also be implemented in C by creating a child process, opening the desired file, redirecting a standard descriptor with `dup2()`, and then calling an `exec` function.

### Example 1: Redirect standard output to a file

Equivalent shell command:

```sh
echo "Hello World!\nGoodbye World!\nHello World Again!" > hello.txt
```

The child opens `hello.txt`, redirects standard output to that file, closes the original descriptor, and then runs `echo`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        // Child process
        /*
            Open the file inside the child process.

            !!!!! It not completely wrong to open it in the parent process. However, the child will
            inherit the file descriptor, so now both the parent and child has to manage their copies
            when they are no longer needed. If you open the file in the child, you can avoid unnecessary
            file descritor management in the parent.
        */
        int output_fd = open("hello.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            // Always check if open() failed.
            perror("open");
            _exit(EXIT_FAILURE);
        }

        /*
            By default the `echo` program will output to standard out (0).

            However, we want it to output to `hello.txt`. Since we already have a
            file descriptor (`output_fd`) that maps to a Open File Table entry for `hello.txt`
            after calling the open() function, what we can do is redirect standard out (0)
            to reference the same Open File Table entry as `output_fd`.
        */
        if (dup2(output_fd, STDOUT_FILENO) < 0) {
            // Always check if dup2() failed.
            perror("dup2");
            _exit(EXIT_FAILURE); // Terminate Child
        }

        /*
            Because we redirected standard out (0) to reference the same
            Open File Table entry as `output_fd`, we essentially have two file descriptors
            referencing the same Open File Table entry. It is best to close the one
            we don't need anymore which is the old `output_fd`.
        */
        close(output_fd);

        // Run the program with exec.
        execlp("echo", "echo", "Hello World!\nGoodbye World!\nHello World Again!", NULL);

        // Should not be here unless exec failed.
        perror("execlp");
        _exit(EXIT_FAILURE); // Terminate Child

    } else {
        // Parent Process
        fprintf(stdout, "Parent waiting for Child: %d\n", pid);
        int status;
        pid_t waited_child = waitpid(pid, &status, 0);
        fprintf(stdout, "Parent done waiting for Child: %d\n", pid);

        // Check if wait failed.
        if (waited_child == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // Check if the waited child is the one the parent spawned.
        if (waited_child == pid) {
            if (WIFEXITED(status)) {
                fprintf(stdout, "Child Exit Code: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(stdout, "Child Killed by Signal: %d\n", WTERMSIG(status));
            }
        }
    }

    return 0;
}
```

### Example 2: Redirect standard input and output

Equivalent shell command:

```sh
grep -i "hello" < hello.txt > matches.txt
```

The child redirects standard input to `hello.txt` and standard output to `matches.txt`, then runs `grep`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        // Child Process

        /*
            This is the file descriptor we will use to read from a file for input.
        */
        int input_fd = open("hello.txt", O_RDONLY);
        if (input_fd < 0) {
            // Always check if open() failed.
            perror("open input");
            _exit(EXIT_FAILURE);
        }

        // Redirect standard in (0) to the Open File Table entry that is referenced by`input_fd`.
        if (dup2(input_fd, STDIN_FILENO) < 0) {
            // Always check if dup2() failed.
            perror("dup2 input");
            _exit(EXIT_FAILURE);
        }
        // Since we redirected standard in (0) to the Open File Table Entry also referenced
        // by `input_fd`, we can close `input_fd` since we don't need it anymore.
        close(input_fd);

        /*
            This is the file descriptor we will use to write to a file for output.
        */
        int output_fd = open("matches.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            // Always check if open failed.
            perror("open output");
            _exit(EXIT_FAILURE);
        }

        // Redirect standard out (1) to the Open File Table entry that is referenced by `output_fd`.
        if (dup2(output_fd, STDOUT_FILENO) < 0) {
            // Always check if dup2() failed.
            perror("dup2 output");
            _exit(EXIT_FAILURE);
        }
        close(output_fd);

        // Run the program with exec.
        execlp("grep", "grep", "-i", "hello", NULL);

        // Should not be here unless exec failed.
        perror("execlp");
        _exit(EXIT_FAILURE); // Terminate Child

    } else {
        // Parent Process
        fprintf(stdout, "Parent waiting for Child: %d\n", pid);
        int status;
        pid_t waited_child = waitpid(pid, &status, 0);
        fprintf(stdout, "Parent done waiting for Child: %d\n", pid);

        // Check if wait failed.
        if (waited_child == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // Check if the waited child is the one the parent spawned.
        if (waited_child == pid) {
            if (WIFEXITED(status)) {
                fprintf(stdout, "Child Exit Code: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(stdout, "Child Killed by Signal: %d\n", WTERMSIG(status));
            }
        }
    }

    return 0;
}
```

## 5. Important Takeaways

- If you need to redirect a program's standard output or another standard descriptor, do the redirection in the **child process before calling `exec()`**.
- If `exec()` succeeds, it replaces the current process image, so code after the `exec()` call does not run.
- Always close file descriptors that are no longer needed. File descriptors are inherited across `fork()` and normally remain open across `exec()` unless marked close-on-exec.
- `dup()` and `dup2()` do **not** copy file contents. They make descriptors refer to the same open file description.
- The operating system closes a process's remaining file descriptors when the process terminates.