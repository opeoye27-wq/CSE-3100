#include <stdio.h>

int main(){
	int a = 10;
	int *p,*q;
	p = &a;
	q = p;
	printf("%p\n",p);
	printf("%p\n",q);
	p++;
	printf("%p\n",p);
	printf("%p\n",q);
}