#include<stdio.h>
#include<string.h>

int main(){
	char d[]="apple";
	printf("length of d is :%lu\n",strlen(d));
	printf("size of d: %lu\n",sizeof(d));
	printf("%c\n",d[3]);
	printf("%c\n",d[5]);
	return 0;
}