#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

typedef struct Operations //holds operation infor
{
    int a; //first argument
    int b; //second argument
    char op; //operation: +, -, *, /, %
    
} Operations;

int main(int argc, char *argv[])
{
    int pa[2]; //parent writes to child
    int ap[2]; //child writes to parent

    //creating pipes
    if (pipe(pa) == -1) {
    perror("Error.");
    return -1;}

    if (pipe(ap) == -1) {
    perror("Error.");
    return -1;}

    int pid = fork(); //creating child process
    if (pid == -1)
    {
        printf("fork() failed\n");
        exit(1);
    }
    if (pid > 0) //parent process
    {
        close(pa[0]);
        close(ap[1]);
        //parent continually reads from txt file until theres an empty line
        //format "(a op b)" will give 6 characters + (\n) ["(", "a", "op", "b", ")", "\0"]
        char *buf[10];
        while (read(1, &buf, sizeof(char) * 6) >= 6)
        {
            Operations *math = malloc(sizeof(Operations)); 
            math->a = atoi(buf[1]);
            math->op = *buf[3];
            math->b = atoi(buf[5]);
            write(pa[1], &math, sizeof(math));

            //parent reads in solution and prints it 
            int s;
            read(ap[0], &s, sizeof(s));
            printf(*buf, " = \n", s);
        }
        close(ap[0]);
        close(pa[1]);
        waitpid(pid, NULL, 0);
        exit(0);
    }

    if (pid == 0) //child process
    {
        close(ap[0]);
        close(pa[1]);
        Operations m;
        while (read(pa[0], &m, sizeof(Operations)) != 0) //continuously read in operations
        {
            int sol;
            if (strcmp(&m.op, "+") != 0)
                sol = m.a + m.b;
            if (strcmp(&m.op, "-") != 0)
                sol = m.a - m.b;
            if (strcmp(&m.op, "*") != 0)
                sol = m.a * m.b;
            if (strcmp(&m.op, "/") != 0)
                sol = m.a / m.b;
            else // % operation
                sol = m.a % m.b;

            write(ap[1], &sol, sizeof(sol)); //write solution back to parent
        }
        close(pa[0]);
        close(ap[1]);
        
    }
}