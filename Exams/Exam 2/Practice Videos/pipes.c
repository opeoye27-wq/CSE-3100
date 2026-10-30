#include <sys/wait.h> //provides functions for waiting on child processes and checking how the terminate (wait(), waitpid())
#include <stdio.h> //provides standard input/ ouput functions (printf(), scanf(), perror())
#include <unistd.h> //provides access to POSIX OS functions (pipe(), forke(), read(), write(), close())
#include <errno.h> //provides access to errno, an integer var to indicate why certain library and sys calls failed
#include <stdlib.h> //provides gen purpose utility functions (malloc(), free(), exit(), atoi(), strol())

int main(int argc, char* argv[])
{
    /*
    Pipes are basically in memory file  that only has a buffer that saves memory
    and you can write and read from it
    */
    //pipe takes in an array of 2 integers (file descriptors) for this pipe which is bascially a key for the file
    int fd[2];
    //fd[0] is always the read end 
    //fd[1] is the write end
    if (pipe(fd) == -1)  //pipes return 0 if successful, -1 if not
    {
        printf("An error occurred with opening the pipe\n");
        return 1;
    } 
    //when you fork the fds get copied over and inherited by the newly created process
    //so if you close an fd in the parent, it remains opened in the child
    int id = fork();
    if (id == -1)
    {
        printf("An error occured with fork()\n");
        return 4;
    }

    if (id == 0) //ask child to input a number and then send it parent process
    {
        //close unused file descriptors
        close(fd[0]);

        int x;
        printf("Input a number: ");
        scanf("%d", &x);
        if (write(fd[1], &x, sizeof(int)) == -1)
        {
            printf("An error occured with writing to the pipe\n");
            return 2;
        }

        //when you're done close the file descriptor
        close(fd[1]);
    }
    else
    {
        //in parent we want to read the integer passed from child
        //first close fd that we don't need 
        close(fd[1]);

        int y;
        if (read(fd[0], &y, sizeof(int)) == -1)
        {
            printf("An error occured with reading from the pipe\n");
            return 3;
        }

        //close fds after using them
        close(fd[0]);

        printf("Got from the child process %d\n", y);
    }

    return 0;
}