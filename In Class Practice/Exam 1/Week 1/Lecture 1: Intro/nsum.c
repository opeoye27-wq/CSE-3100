/* task 1 - take n as an input */
/* task 2 - compute sum of number from 1 to n  */
/* task 3 - print the sum */

#include<stdio.h>

int main(){
	int n,i;
	printf("Enter n:");
	scanf("%d",&n);
	printf("n = %d\n",n);
	i = 1;
	int sum = 0;
	while(i <= n){
		sum = sum+i;
		/* printf("sum = %d\n",sum); */
		i = i+1;
		/* printf("i = %d\n",i); */
	}
	printf("Sum of %d numbers is %d\n",n,sum);
	return 0;
}
