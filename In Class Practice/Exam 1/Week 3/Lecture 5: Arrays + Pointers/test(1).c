#include <stdio.h>

void test(int a) {
  a  = a+1;
  printf("a in test = %d\n", a);
}

int main() {
  int a = 10;
  printf("a in main before test = %d\n", a);
	test(a);
  printf("a in main after test = %d\n", a);
}