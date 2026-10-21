# CSE-3100
# Systems Programming

A collection of coursework, labs, homework assignments, and practice exercises for Systems Programming.

## Overview

This repository contains my work throughout the Systems Programming course, including programming assignments, laboratory exercises, and code written to practice low-level programming concepts in C.

The goal of this repository is to organize my coursework and track my progress in understanding how programs interact with the operating system and manage system resources.

## Topics Covered

Topics practiced throughout the course include:

- C programming and command-line arguments
- Pointers, arrays, strings, and memory management
- Processes and process creation using `fork()`
- Program execution using `exec()` family functions
- File descriptors and I/O redirection using `dup()` and `dup2()`
- Process synchronization and waiting using `wait()` and `waitpid()`
- Linked lists and dynamic memory allocation
- Debugging and memory error detection
- Compilation, Makefiles, and command-line tools

## Technologies and Tools

- **Language:** C
- **Compiler:** GCC / Clang
- **Environment:** Linux / Unix-like systems
- **Build tools:** Make and Makefiles
- **Version control:** Git and GitHub

## Compiling and Running Programs

Compilation instructions may vary by assignment. For a typical C source file, you can compile and run it using:

```bash
gcc -Wall -Wextra -std=c99 program.c -o program
./program
```

For assignments that include a `Makefile`, use:

```bash
make
```

Then run the executable specified by the assignment.

## Notes

- Individual assignments may have their own instructions and compilation requirements.
- Some programs may require command-line arguments or specific input files.
- Coursework may depend on the Linux environment or tools provided for the course.

## Academic Use

This repository is intended to document my learning and practice throughout the course. Please follow your institution's academic integrity policies when referencing or using coursework.
