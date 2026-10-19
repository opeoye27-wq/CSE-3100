#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[])
{
	if(argc != 2) //must take 2 arguments
	{
		printf("Usage: %s n\n", argv[0]);
		return -1;
	}
	int n = atoi(argv[1]); //converts the second passed argument (string) to an integer
	int sum = 0;
	for(int i=1; i<=n; i++)
	{
		sum += i; //iteratively add first n numbers
	}
	printf("sum = %d\n", sum);
	return 0;
}

