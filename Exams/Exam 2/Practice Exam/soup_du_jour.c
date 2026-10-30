#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Must pass one character\n");
        return 1;
    }
    int pa[2]; //parent writes, childA reads
    int ap[2]; //childA writes, parent reads
    int ab[2]; //A writes, B reads
    int ba[2]; //B writes, A reads
    int pb[2]; //parent writes, B reads
    int bp[2]; //B writes, parent reads

    if (pipe(pa) == -1) {
    perror("Error.");
    return -1;}

    if (pipe(ap) == -1) {
    perror("Error.");
    return -1;}

    if (pipe(ab) == -1) {
    perror("Error.");
    return -1;}

    if (pipe(ba) == -1) {
    perror("Error.");
    return -1;}

    if (pipe(pb) == -1) {
    perror("Error.");
    return -1;}

    if (pipe(bp) == -1) {
    perror("Error.");
    return -1;}

    int a = fork(); //childA

    if (a == -1)
    {
        printf("fork() failed\n");
        exit(1);
    }
    
    if (a == 0) //inside childA
    {
        //close unused file descriptors
        close(pa[1]);
        close(ap[0]);
        close(ab[0]);
        close(ba[1]);
        close(pb[0]);
        close(pb[1]);
        close(bp[0]);
        close(bp[1]);

        //read letter passed from parent
        char s;
        read(pa[0], &s, sizeof(char));
        printf("Child 1 read: %c\n", s);

        char next_s = s + 1; //next letter to send to B
        close(pa[0]);

        //send next letter to B
        write(ab[1], &next_s, sizeof(next_s));
        printf("Child 1 write: %c\n", next_s);
        close(ab[1]);

        //Read letter sent by B
        char n;
        read(ba[0], &n, sizeof(char));
        printf("Child 1 read: %c\n", n);
        close(ba[0]);

        //send next letter to parent to print
        char next_n = n + 1;
        write(ap[1], &next_n, sizeof(next_n));
        printf("Child 1 wrote: %c\n", next_n);
        close(ap[1]);
    }
    else
    {
        int b = fork(); //childB
        if (b == -1)
        {
            printf("fork() failed\n");
            exit(1);
        }

        if (b == 0)
        {
            //close unused file descriptors
            close(pa[0]);
            close(pa[1]);
            close(ap[0]);
            close(ap[1]);
            close(ab[1]);
            close(ba[0]);
            close(pb[0]);
            close(bp[1]);

            //read letter passed from A
            char s;
            read(ab[0], &s, sizeof(char));
            printf("Child 2 read: %c\n", s);
            close(ab[0]);
            char next_s = s + 1; //next letter to send to B
        

            //send next letter to parent
            write(pb[1], &next_s, sizeof(next_s));
            printf("Child 2 wrote: %c\n", next_s);
            close(pb[1]);

            //read letter sent back from parent
            char n;
            read(bp[0], &n, sizeof(char));
            printf("Child 2 read: %c\n", n);
            close(bp[0]);

            //send next letter to A
            char next_n = n + 1;
            write(ba[1], &next_n, sizeof(next_n));
            printf("Child 2 wrote: %c\n", next_n);
            close(ba[1]);
        }
       
        else //parent
        {
            //close unused file descriptors
            close(pa[0]);
            close(ap[1]); 
            close(ab[0]);
            close(ab[1]);
            close(ba[0]);
            close(ba[1]);
            close(pb[1]);
            close(bp[0]);

            //parent reads letter from command line and sends it to A
            char s = argv[1][0]; //read the first character of string

            write(pa[1], &s, sizeof(s));
            printf("Parent wrote: %c\n", s);
            close(pa[1]); //close fds you won't need anymore

            //Parent reads letter from B
            char n;
            read(pb[0], &n, sizeof(n));
            printf("Parent read: %c\n", n);
            close(pb[0]);

            //send letter back to B
            char next_n = n + 1;
            write(bp[1], &next_n, sizeof(next_n));
            printf("Parent wrote: %c\n", next_n);
            close(bp[1]);

            //read final letter from A to print
            char f;
            read(ap[0], &f, sizeof(char));
            printf("Parent read: %c\n", f);
            close(ap[0]);

            waitpid(a, NULL, 0);
            waitpid(b, NULL, 0);
            exit(0);
        }
    }
}