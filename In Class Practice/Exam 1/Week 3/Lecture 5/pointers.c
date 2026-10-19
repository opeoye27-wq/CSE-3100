#include <stdio.h>

int main() {

  int a = 10;
  printf("a = %d\n", a);
  printf("address of a = %p\n", &a);

  /* int *pa = &a; */
  int *pa;
  pa = &a;

	char c = 'z';
	char *pc = &c;

  printf("pa = %p\n", pa);
  printf("*pa = %d\n", *pa);

  *pa = 12;
  printf("*pa = %d\n", *pa);

  printf("a = %d\n", a);

  return 0;
}