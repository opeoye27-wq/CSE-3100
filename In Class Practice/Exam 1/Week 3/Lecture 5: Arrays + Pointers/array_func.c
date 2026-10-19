#include<stdio.h>

void test(int x[],int n){
	x[0]+=10;
	printf("x[0] in test()  = %d\n",x[0]);
}

int main(){
	int x[4]={1,2,3,4};
	printf("x[0] in main before test() = %d\n",x[0]);
	test(x,4);	
	printf("x[0] in main after test()=%d\n",x[0]);
	return 0;
}
