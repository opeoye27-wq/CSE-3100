#include <stdio.h>

int main() {

	// 10,20,30,40,50
	
	int arr[5]; //define an array
	arr[0] = 10;
	arr[1] = 20;
	arr[2] = 30;
	arr[3] = 40;
	arr[4] = 50;

	int y[5]={10,20,30,40,50};
	int z[] = {10,20,30,40,50};

	printf("arr[1] = %d\n",arr[1]);
	printf("y[3] = %d\n",y[3]);
	int a = 10;
	int b;
	b = a;
	int temp[5];
	for(int i=0;i<5;i++){
		temp[i] = arr[i];
	}

	printf("temp[1] = %d\n",temp[1]);
	return 0;
}