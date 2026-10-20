#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t child;
    int exitStatus;

    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // TODO
    child = fork();
    if (child == -1)
	{ 
        perror("fork()");
        exit(EXIT_FAILURE);
    }

    if(child == 0) //we're in the first child
    {
        //pass in the first 2 arguments after function call
        execlp(argv[1], argv[1], argv[2], (char *)NULL);

        perror("execlp()"); //only executes if it doesn't work
        _exit(EXIT_FAILURE);
    }
    
    else 
    {
        // Parent process
        if (wait(&exitStatus) == -1) //make sure our child process terminated properly
        {
            perror("wait()");
            exit(EXIT_FAILURE);
        }
        
        printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WIFEXITED(exitStatus) ? WEXITSTATUS(exitStatus) : -1);
        } 

    pid_t child2 = fork(); //creating second process

    if (child2 == -1)
	{ 
        perror("fork()");
        exit(EXIT_FAILURE); 
    } 

    if (child2 == 0)// second process to execute remaining arguments
    {
         //make a pointer to array starting at 3rd argument
        execvp(argv[3], &argv[3]);

        perror("execvp()"); //only executes if it doesn't work
        _exit(EXIT_FAILURE);
    }

    else 
    {
        // Parent process
        if (wait(&exitStatus) == -1) //make sure our child process terminated properly
        {
            perror("wait()");
            exit(EXIT_FAILURE);
        }
        
        printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WIFEXITED(exitStatus) ? WEXITSTATUS(exitStatus) : -1);
        } 
    return 0;
}