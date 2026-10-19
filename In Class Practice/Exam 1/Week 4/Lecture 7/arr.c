#include<stdio.h>

int main(){
	int arr[10]={1,2,3,4,5,6};
	int *p = &arr[5];
	int *q = arr;
	printf("%d\n",arr[1]);
	printf("%d\n",arr[4]);
	printf("%d\n",arr[-1]);
	printf("%d\n",arr[-10]);
	printf("%d\n",p[-1]);
	printf("%d\n",q[-10]);

	return 0;
}