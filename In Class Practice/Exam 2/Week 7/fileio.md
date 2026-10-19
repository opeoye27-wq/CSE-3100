# File I/O in C

# 1. High-Level File I/O

## 1.1 Overview

High-level file I/O is provided by the standard C library, `stdio.h`, which abstracts low-level file I/O operations.

Instead of working directly with file descriptors, you work with **streams**, represented by `FILE *`.

The `FILE` object is not the actual file. It is managed by the library to help interact with the file.

High-level file I/O is more convenient because it provides **buffering**, which reduces the number of system calls needed to read or write data.

A `FILE` object stores information such as:

- File descriptor
- I/O buffer
- Current stream position
- End-of-file indicator

## High-Level I/O Functions

```c
#include <stdio.h>

// Open and close files
FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *stream);

// Character I/O
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);

// String I/O
char *fgets(char *buf, int buf_size, FILE *stream);
int fputs(const char *str, FILE *stream);

// Formatted I/O
int fscanf(FILE *stream, const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);

// Block I/O: read or write chunks of data
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

// File positioning
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
```

### Default File Streams

C provides three standard streams:

| Stream | Purpose |
|---|---|
| `stdin` | Standard input |
| `stdout` | Standard output |
| `stderr` | Standard error output |

---

## 1.2 Opening and Closing Files: `fopen()` and `fclose()`

### Function Prototypes

```c
#include <stdio.h>

FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *stream);
```

| Function | Description | Return Value |
|---|---|---|
| `fopen()` | Opens a file using the specified filename and mode. | Success: pointer to a `FILE` stream. Failure: `NULL`. |
| `fclose()` | Closes an open `FILE` stream. | Success: `0`. Failure: `EOF`. |

### File Open Modes

| Mode | Read? | Write? | If File Exists | If File Does Not Exist | Initial Position |
|---|---|---|---|---|---|
| `"r"` | Yes | No | Opens normally | Fails (`NULL`) | Beginning |
| `"r+"` | Yes | Yes | Opens normally | Fails (`NULL`) | Beginning |
| `"w"` | No | Yes | Truncates file to 0 bytes | Creates file | Beginning |
| `"w+"` | Yes | Yes | Truncates file to 0 bytes | Creates file | Beginning |
| `"a"` | No | Yes | Appends to file | Creates file | End |
| `"a+"` | Yes | Yes | Appends writes to file | Creates file | End for writing; reading position depends on the implementation |

**Important things to remember:**

- `"r"` requires the file to exist.
- `"w"` erases existing file contents.
- `"a"` writes new data at the end of the file.
- The `+` allows both reading and writing.
- Always check whether `fopen()` returns `NULL`.
- Close files when you are finished using `fclose()`.

---

## 1.3 Character I/O: `fgetc()` and `fputc()`

### Function Prototypes

```c
#include <stdio.h>

int fgetc(FILE *stream);
int fputc(int c, FILE *stream);
```

| Function | Description | Return Value |
|---|---|---|
| `fgetc()` | Reads one character from a file stream and advances the file position. | Returns the character as an `int`, or `EOF` at end-of-file or on error. |
| `fputc()` | Writes one character to a file stream and advances the file position. | Returns the character written, or `EOF` on failure. |

### Key Points

- `fgetc()` returns an `int` so it can represent every possible `unsigned char` value as well as `EOF`.
- When storing the result of `fgetc()`, use an `int` so you can correctly check for `EOF`.
- Both functions advance the stream position.

---

## 1.4 String I/O: `fgets()` and `fputs()`

### Function Prototypes

```c
#include <stdio.h>

char *fgets(char *buf, int buf_size, FILE *stream);
int fputs(const char *str, FILE *stream);
```

| Function | Description | Return Value |
|---|---|---|
| `fgets()` | Reads characters into a buffer until a newline is read, EOF occurs, or `buf_size - 1` characters have been read. | Success: pointer to the destination buffer. Failure or no characters read before EOF: `NULL`. |
| `fputs()` | Writes a null-terminated string to a file stream. | Success: nonnegative value. Failure: `EOF`. |

### Important Notes

**`fgets()`**

- Reads at most `buf_size - 1` characters, leaving room for the terminating `'\0'`.
- Adds a null terminator to the buffer when it successfully reads a string.
- Keeps the newline character `'\n'` in the buffer if it reads one.
- Advances the file position by the number of characters read.

**`fputs()`**

- Uses the null terminator `'\0'` to determine where the input string ends.
- Does not write the terminating `'\0'` to the file.
- Does not automatically add a newline at the end.

---

## 1.5 Formatted I/O: `fprintf()` and `fscanf()`

### Function Prototypes

```c
#include <stdio.h>

int fscanf(FILE *stream, const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
```

| Function | Description | Return Value |
|---|---|---|
| `fscanf()` | Reads formatted data from a file and stores parsed values in variables. | Returns the number of input items successfully matched and assigned. Returns `EOF` if input fails before the first conversion. |
| `fprintf()` | Writes formatted data to a file stream. | Returns the number of characters written, or a negative value on failure. |

### `fscanf()` Parameters

- `stream`: the open file stream to read from.
- `format`: the format string describing the expected input.
- `...`: addresses of the variables where the parsed values will be stored.

Example:

```c
int age;
fscanf(file, "%d", &age);
```

Notice that `&age` is used because `fscanf()` needs the address where it should store the value.

### `fprintf()` Parameters

- `stream`: the open file stream to write to.
- `format`: the format string describing the output.
- `...`: the values to insert into the formatted output.

Example:

```c
int age = 20;
fprintf(file, "Age: %d\n", age);
```

### Important Note About `%s`

When `fscanf()` uses `%s` to read a string:

- It adds a null terminator to the destination buffer.
- It stops reading at whitespace.
- It does not know the size of the destination buffer.

This means `%s` can overflow a buffer if the input is too long. Use a width limit, such as `%19s` for a buffer of at least 20 bytes, or use a safer input strategy.

---

## 1.6 Block I/O: `fread()` and `fwrite()`

These functions read and write blocks of data rather than processing one character or formatted value at a time.

### Function Prototypes

```c
#include <stdio.h>

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
```

| Function | Description | Return Value |
|---|---|---|
| `fread()` | Reads data from a file stream into a memory buffer. | Returns the number of complete elements read. |
| `fwrite()` | Writes data from a memory buffer to a file stream. | Returns the number of complete elements written. |

### Parameters

| Parameter | Meaning |
|---|---|
| `ptr` | Pointer to the buffer holding the data or receiving the data. |
| `size` | Size of each element in bytes. |
| `nmemb` | Number of elements to read or write. |
| `stream` | The open file stream. |

### Understanding the Return Value

- If the requested number of elements is processed successfully, the return value equals `nmemb`.
- If fewer elements are processed, the return value is smaller than `nmemb`.
- A short read may occur because of EOF or an error.
- A short write indicates that fewer elements were written than requested.

**Remember:** `fread()` and `fwrite()` return the number of complete elements processed, not the total number of bytes.

---

## 1.7 File Positioning: `fseek()`, `ftell()`, and `rewind()`

These functions let you inspect or change the current position in a file stream.

### Function Prototypes

```c
#include <stdio.h>

int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
```

| Function | Description | Return Value |
|---|---|---|
| `fseek()` | Moves the file position indicator to a specified location. | Success: `0`. Failure: nonzero value. |
| `ftell()` | Reports the current file position. | Success: current position as a `long`. Failure: `-1L`. |
| `rewind()` | Moves the file position to the beginning and clears the stream's EOF and error indicators. | No return value. |

### The `whence` Argument

The `whence` argument specifies the reference point for the offset.

| Constant | Meaning |
|---|---|
| `SEEK_SET` | Beginning of the file |
| `SEEK_CUR` | Current position |
| `SEEK_END` | End of the file |

### Example

```c
fseek(file, 0, SEEK_SET);  // Move to the beginning
fseek(file, 5, SEEK_CUR);  // Move forward 5 bytes
fseek(file, 0, SEEK_END);  // Move to the end

long position = ftell(file);  // Get current position

rewind(file);  // Return to the beginning
```

`fseek()` allows nonsequential, or random, access to file contents instead of requiring you to read the file from beginning to end.

---

# 2. Low-Level File I/O

## 2.1 Overview

Low-level file I/O interacts more directly with the operating system kernel through system calls.

Instead of using `FILE *` streams, low-level I/O uses **file descriptors**, which are integer values identifying open files or other I/O resources.

Low-level I/O is less abstracted than high-level I/O and requires you to manage byte counts, buffers, and errors more directly.

### Why Buffering Matters

System calls have overhead. If a program reads or writes lots of small chunks, it may make many system calls and run more slowly.

High-level I/O uses buffering to reduce these calls. For example, it may collect multiple small writes in a buffer and make a system call when the buffer needs to be flushed.

### Low-Level I/O Functions

```c
#include <fcntl.h>   // open() and O_* flags
#include <unistd.h>  // read(), write(), close(), lseek()

// Open and close files
int open(const char *pathname, int flags, ...);
int close(int fd);

// Read and write bytes
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

// File positioning
off_t lseek(int fd, off_t offset, int whence);
```

The `open()` function accepts a third argument, `mode_t mode`, when creating a file with `O_CREAT`.

### Default File Descriptors

| File Descriptor | Purpose |
|---|---|
| `0` | Standard input |
| `1` | Standard output |
| `2` | Standard error |

These are commonly accessed through `stdin`, `stdout`, and `stderr` when using high-level I/O.

---

## 2.2 Opening and Closing Files: `open()` and `close()`

### Function Prototypes

```c
#include <fcntl.h>
#include <unistd.h>

int open(const char *pathname, int flags, ...);
int close(int fd);
```

| Function | Description | Return Value |
|---|---|---|
| `open()` | Opens a file and returns a file descriptor identifying it. | Success: lowest available nonnegative file descriptor. Failure: `-1`, with `errno` set. |
| `close()` | Closes an open file descriptor, releasing that descriptor for reuse. | Success: `0`. Failure: `-1`, with `errno` set. |

### `open()` Parameters

- `pathname`: null-terminated path to the file.
- `flags`: options controlling how the file is opened.
- `mode`: file permissions used when creating a new file with `O_CREAT`.

### Important `open()` Flags

| Flag | Description |
|---|---|
| `O_RDONLY` | Open for reading only. |
| `O_WRONLY` | Open for writing only. |
| `O_RDWR` | Open for both reading and writing. |
| `O_CREAT` | Create the file if it does not exist. |
| `O_TRUNC` | Truncate an existing regular file to zero bytes when opened with suitable write access. |
| `O_APPEND` | Append writes to the end of the file. |
| `O_CLOEXEC` | Set close-on-exec for the file descriptor so it is closed when an `exec` operation succeeds. |

Choose exactly one of `O_RDONLY`, `O_WRONLY`, and `O_RDWR` as the access mode. Other flags can be combined with the access mode using the bitwise OR operator `|`.

### Example

```c
int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);

if (fd == -1) {
    perror("open");
}
```

Here, the file is opened for writing and created if it does not exist. The permission mode `0644` applies when a new file is created.

---

## 2.3 File Permissions: `mode_t mode`

When using `O_CREAT`, you specify the permissions for a newly created file.

A common example is:

```c
open("example.txt", O_WRONLY | O_CREAT, 0644);
```

The permission value `0644` is written in **octal (base 8)**.

### Understanding `0644`

| Digit | Applies To | Value |
|---|---|---|
| First digit: `0` | Octal notation | Indicates the number is written in base 8. |
| Second digit: `6` | Owner / user | Read and write |
| Third digit: `4` | Group | Read only |
| Fourth digit: `4` | Others | Read only |

### Calculating Permission Values

Each permission digit is calculated by adding the following values:

| Value | Permission | Symbol |
|---|---|---|
| `4` | Read | `r` |
| `2` | Write | `w` |
| `1` | Execute | `x` |
| `0` | No permission | `-` |

### Permission Combinations

| Value | Calculation | Permissions |
|---|---|---|
| `7` | `4 + 2 + 1` | Read, write, execute (`rwx`) |
| `6` | `4 + 2 + 0` | Read and write (`rw-`) |
| `5` | `4 + 0 + 1` | Read and execute (`r-x`) |
| `4` | `4 + 0 + 0` | Read only (`r--`) |
| `3` | `0 + 2 + 1` | Write and execute (`-wx`) |
| `2` | `0 + 2 + 0` | Write only (`-w-`) |
| `1` | `0 + 0 + 1` | Execute only (`--x`) |
| `0` | `0 + 0 + 0` | No permissions (`---`) |

### Example: `0644`

`0644` corresponds to `rw-r--r--`:

- **Owner:** read and write (`rw-`)
- **Group:** read only (`r--`)
- **Others:** read only (`r--`)

The permissions actually applied to a new file can also be restricted by the process's `umask`.

---

## 2.4 Reading and Writing: `read()` and `write()`

These functions transfer raw bytes using file descriptors.

### Function Prototypes

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

| Function | Description | Return Value |
|---|---|---|
| `read()` | Reads up to `count` bytes from a file descriptor into a buffer. | Success: number of bytes read. `0`: EOF. Failure: `-1`, with `errno` set. |
| `write()` | Writes up to `count` bytes from a buffer to a file descriptor. | Success: number of bytes written. Failure: `-1`, with `errno` set. |

### Parameters

| Parameter | Meaning |
|---|---|
| `fd` | The open file descriptor. |
| `buf` | Pointer to the memory buffer used for reading or writing. |
| `count` | Maximum number of bytes to read or number of bytes requested for writing. |

### Important Notes About `read()`

- It may read fewer bytes than requested.
- A return value of `0` means EOF for a regular file.
- A return value of `-1` indicates an error.
- The returned byte count tells you how much data was actually read.

### Important Notes About `write()`

- It may write fewer bytes than requested.
- A short write can occur for reasons such as interruptions or resource limitations.
- A successful return does not necessarily mean all requested bytes were written.
- The return value tells you how many bytes were actually written.

**Remember:** Both functions work with bytes, not C strings. They do not automatically add a null terminator to buffers.

---

## 2.5 File Positioning: `lseek()`

`lseek()` changes the file offset associated with a file descriptor, allowing nonsequential reading or writing.

### Function Prototype

```c
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
```

| Function | Description | Return Value |
|---|---|---|
| `lseek()` | Repositions the file offset associated with a file descriptor. | Success: resulting offset from the beginning of the file. Failure: `(off_t) -1`, with `errno` set. |

### Parameters

- `fd`: the open file descriptor.
- `offset`: the offset used to calculate the new position.
- `whence`: the reference point for the offset.

### The `whence` Argument

| Constant | Meaning |
|---|---|
| `SEEK_SET` | Beginning of the file |
| `SEEK_CUR` | Current file offset |
| `SEEK_END` | End of the file |

### Example

```c
lseek(fd, 0, SEEK_SET);  // Move to the beginning
lseek(fd, 5, SEEK_CUR);  // Move forward 5 bytes
lseek(fd, 0, SEEK_END);  // Move to the end

off_t position = lseek(fd, 0, SEEK_CUR);  // Get current offset
```

The returned value is the new file offset measured in bytes from the beginning of the file.

`off_t` is the type used to represent file offsets.

---

# 3. High-Level vs. Low-Level File I/O

| Feature | High-Level I/O | Low-Level I/O |
|---|---|---|
| Main representation | `FILE *` stream | Integer file descriptor |
| Header | `<stdio.h>` | `<fcntl.h>` and `<unistd.h>` |
| Open a file | `fopen()` | `open()` |
| Close a file | `fclose()` | `close()` |
| Read data | `fgetc()`, `fgets()`, `fscanf()`, `fread()` | `read()` |
| Write data | `fputc()`, `fputs()`, `fprintf()`, `fwrite()` | `write()` |
| Change file position | `fseek()` | `lseek()` |
| Get current position | `ftell()` | `lseek(fd, 0, SEEK_CUR)` |
| Buffering | Library-managed buffering | Direct system calls; no `stdio` stream buffering |
| Typical advantage | Convenient and easier to use | More direct control over byte-level I/O |

## 4. Quick Review: What to Remember

### High-Level I/O

- `FILE *` represents a stream managed by the C library.
- `fopen()` opens a stream; `fclose()` closes it.
- `"r"` reads, `"w"` writes and truncates, and `"a"` appends.
- `fgets()` reads a bounded amount of text and retains a newline if it reads one.
- `fputs()` does not automatically add a newline.
- `fscanf()` reads formatted data; `fprintf()` writes formatted data.
- `fread()` and `fwrite()` process blocks of elements.
- `fseek()`, `ftell()`, and `rewind()` manage stream positions.

### Low-Level I/O

- File descriptors are integers.
- `open()` opens a file; `close()` closes a file descriptor.
- `read()` and `write()` operate on bytes.
- A successful `read()` or `write()` may process fewer bytes than requested.
- `lseek()` changes the file offset.
- `O_CREAT` creates a missing file; `O_TRUNC` truncates an existing file; `O_APPEND` appends writes.
- Permission values such as `0644` use octal notation.
- Check return values for errors, and use `perror()` to report system-call errors.

**Main takeaway:** High-level I/O gives you convenient, buffered stream operations. Low-level I/O gives you more direct control through file descriptors and system calls.