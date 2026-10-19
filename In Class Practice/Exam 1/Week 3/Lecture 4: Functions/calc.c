#include <stdio.h>

int add(int,int);
float div(float, float);

int main() {

	int a = 2, b = 3;
	int sum = add(a,b);
	printf("sum = %d\n",sum);

	float res = div(a,b);
	printf("res = %f\n",res);
  return 0;
}


int add(int a, int b) {
  int result;
  result = a + b;
  return result;
}

float div(float c, float d){
	return c/d;
}