// Only this line of comment is provided 
#include <stdio.h>
int main(void)
{
	double x;
	double total = 0;
	double average;
	int count = 0;
	while (scanf("%lf", &x) == 1) {
		 total = total + x;
		 count = count + 1;
		 average = total / count;
		 printf("Total=%f Average=%f\n", total, average);
		}
	return 0;
}
