//In this assignment, we use fork() to craete multiple processes to solve a puzzle.
//The puzzle is specified by the following array
//int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};
//A walker walks along the index of this array.
//He starts at the index 0, where a[0] is 3, which indicates that
//the walker can move 3 steps. If the walker move to the left, he would be at index
//-3, which is out of the range. Hence, he can only move to the right, after he makes the move,//he will be at index 3. Since a[3] = 1, he can move to the left or right by one move.

//Note whenever he makes his move, he should be in the range [0, 9]. He is not allowd to move 
//outside of this range.

//The goal of the puzzle is to for the walker to reach index 9, where a[9] = 0. And he will
//stop there.

//What we need to do is to find the solutions of this puzzle. We limit the solutions to have
//at most 10 moves.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
void print_solution(int b[], int moves)
{
    for(int k = 0; k<moves; k++)
    {
        printf("->%d ", b[k]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    //We use the array a to describe the puzzle
    int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};

    //We use the array b to save the moves the walker makes
    //Essentially, we save each index that the walker reaches in array b
    //For example, b[0] = 3 since the walker goes to index 3 for the first move
    
    //both children and parent get a copy of all the code until fork()
    int b[10]; //index the walker reaches   
    int cur = 0; //current index we are at
    int moves = 0; //number of moves we've done, max is 10

    int n = 10; //max moves

    for(int i = 0; i< n; i++) 
    {
		pid_t pid;
    	pid = fork(); //create a process
        if(pid == 0) //child process
        {   
            //If we find a solution, we print the solution 
            if(a[cur]==0) //if we're at the end of the array
            {
                b[moves - 1] = cur; //set the last move to be current
                print_solution(b, moves); //call print_solution
                return 0;
            }
            else if(cur + a[cur] >= 0 && cur + a[cur] <n) //if current + next move >= 0 and still under max moves (moving right)
            {
                //Add your code here
                cur = cur + a[cur]; //move right to next spot
                b[moves] = cur; //record index we've moved to
                moves++; //increase number of moves
            }
        }
        else
        {
			int status;
			//wait for the child process to finish
			waitpid(pid, &status, 0);
            //If we find a solution, we print the solution
            if(a[cur]==0)
            {
                b[moves - 1] = cur;
                print_solution(b, moves);
                return 0;
            }
            else if(cur - a[cur] >= 0 && cur - a[cur] <n) //if current - next move >= 0 and still under max moves (moving left)
            {
                //Add your code here
                cur = cur - a[cur];
                b[moves] = cur;
                moves++;
            }
        }
    }
    return 0;
}
/*
Note run your code the following different ways and observe the outputs
./fork-puzzle
./fork-puzzle |sort
./fork-puzzle |sort| uniq
*/