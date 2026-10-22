//In this assignment, we practice dup2() and fork().
//The main() function takes a command line argument.
//In the main() function we use fork() to create a
//child process. The child process redirects the
//standard output to a file named "output.txt" using dup2().
//Moreover, it prints out the command line argument in capital letters.
//The parent process simply prints out the command line argument. 

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <ctype.h> 
#include <sys/stat.h>
#define MAX_LINE 1024

int main(int argc, char *argv[])
{

	if(argc!=2) //check if we're given exactly 2 command line arguments
	{
		printf("Usage: %s message\n", argv[0]);
		return -1; 
	}
    pid_t pid;
    pid = fork();
    if(pid == 0) //in the Child process
    {

		//Make sure when the file is created, the user has the read and write access 
		int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); //opening the output file
		/*
			O_WRONLY Open the file for writing only.
			O_CREAT	Create the file if it doesn't exist.
			O_TRUNC	Erase the existing contents if the file already exists.
			S_IRUSR	Give the owner read permission when creating the file.
			S_IWUSR	Give the owner write permission when creating the file.
		*/
		if(fd < 0) //checking if succesfully opened
		{ 
			printf("Cannot open the file\n");
			return -1;
		} 
        char buffer[MAX_LINE];
        strcpy(buffer, argv[1]); //copy passed argument into a buffer (ends in '\0)
		//TODO
		//fill in the code below

		//redirect output using dup2
		//check if dup2() worked
		if (dup2(fd, 1) == -1)
		{
			perror("dup2()");
			close(fd);
			return 1;
		}
		close(fd);

		int i = 0;
    	while (buffer[i] != '\0')
		{
        	buffer[i] = toupper((unsigned char)buffer[i]); //toupper will change the lowercase letters to uppercase
        	i++;
		}
		printf("%s\n",buffer);
		return 0;
    }

	printf("%s\n", argv[1]);
	return 0;
} 

