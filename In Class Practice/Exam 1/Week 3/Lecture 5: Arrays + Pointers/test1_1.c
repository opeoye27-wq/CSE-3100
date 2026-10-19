#include<stdio.h>

void test(int *a){
	*a = 11;
}
int main(){
	int a = 10;
	printf("a before calling test():%d\n",a);
	test(&a);
	printf("a after calling test():%d\n",a);
	return 0;
}