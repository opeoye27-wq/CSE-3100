#include <stdio.h>

void test(int *p) { printf("size of p in test():%lu\n", sizeof(p)); }

int main() {
  int a[10];
  int *p = a;

  printf("size of a:%lu\n", sizeof(a));
  printf("size of p:%lu\n", sizeof(p));
	test(a);
	test(p);
}