#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n, i;

	printf("n = ");
	scanf("%d", &n);

	double pi = 0.;
	//TODO
	//add code below 
	for(i = 0; i <= n; i++)
	{
		double current = 0;
		current = (4.0/ ((8 * i) + 1)) - (2.0/ ((8 * i) + 4)) - (1.0/ ((8 * i) + 5)) - (1.0/ ((8 * i) + 6));

		int power = 1;
		int j = 1;
		while(j <= i)
		{
			power *= 16;
			j += 1;
		}

		current *= 1.0 / power;
		pi += current;
	}
	
	printf("PI = %.10f\n", pi);
	return 0;
}