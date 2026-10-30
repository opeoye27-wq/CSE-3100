#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[])
{
    /*
    2 way communication between a parent and a child 
    generates a number 0-9 sent to parent multiplies it by 4 and then returned to child to print
    need 2 pipes
    */
   int p1[2];
   if (pipe(p1) == -1) {return -1;} //make the first pipe

   int pid = fork(); //create processes

   if (pid == -1) {return 2;}

   if (pid == 0) //child process
   {
        int x;
        if (read(p1[0], &x, sizeof(x)) == -1) //read in a integer from parent
            return 3;
        printf("Received %d\n", x);
        x *= 4;
        if (write(p1[1], &x, sizeof(x)) == -1) //after multiplying by 4, write that integer back to parent
            return 4;
        printf("Wrote%d\n", x);

   }
   else //parent process
   {
    //generate the random number
    srand(time(NULL));
    int y = rand() % 10; //gives us numbers 0-9 (inclusive)
    //write the number to the child process who will multiply it by 4
    if (write(p1[1], &y, sizeof(y)) == -1)
        return 5;

    printf("Wrote %d\n", y);

    //read the multiplied number back from child
    if(read(p1[0], &y, sizeof(y)) == -1)
        return 6;
    //print result that was read in
    printf("Result is %d\n", y);

    wait(NULL);
   }
   close(p1[0]);
   close(p1[1]);
    
    return 0;
}