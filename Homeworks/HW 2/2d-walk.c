#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{

	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.

	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.

	int x = 0; //increase/decrease for left/right
	int y = 0; // increase/ decrease for up/down
	int count = 0;

	int size = 2 * n + 1;
	int visited[size][size]; //creating the array

	//initializing the array
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			visited[i][j] = 0;
		}
	}

	//count starting point (0, 0) as visited
	visited[x + n][y + n] = 1;
	count ++;

	//loop until we go outside of the square
	while(x > -n && x < n && y > -n && y < n)
	{
		int r = rand() % 4;

		if(r == 0) // up
			y ++;
		else if(r == 1) //right
			x++;
		else if(r == 2) //down
			y--;
		else if(r == 3) //left
			x--;
		
		//Check if we're still in the square
		if(x > -n && x < n && y > -n && y < n)
		{
			if(visited[x + n][y + n] == 0) // we haven't visited this yet
			{
				visited[x + n][y + n] = 1;
				count ++;
			}
		}
	}
	//total num of points inside the square
	int total = (2 * n - 1) * (2 * n - 1);
	return (double)count / total;
}

//Do not change the code below
int main(int argc, char *argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}
