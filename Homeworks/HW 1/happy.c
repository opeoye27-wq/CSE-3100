#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n;

	printf("n = ");
	scanf("%d", &n);

	int m = n; //Original number
	//TODO
	//add code below
	while(n != 4 && n != 1)
	{
		int current = n; // make a copy of n before calculations
		int sum = 0;
		while(current)
		{
			int last = current % 10; // get the last digit
			int sq = last * last;
			sum += sq;
			current = current / 10; // divides current by 10
		}
		n = sum;
		printf("%d\n", n);
		
	}






	if(n==1) printf("%d is a happy number.\n", m);
	else printf("%d is NOT a happy number.\n", m);
	return 0;
}