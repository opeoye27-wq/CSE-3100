#include <stdio.h>

int *test() {
  int a = 234;
  printf("The address of a = %p\n", &a);
  return &a;
}
void t() {
  int b = 10;
  printf("The address of b  = %p\n", &b);
  printf("B is %d\n", b);
}
int main(int argc, char **argv) {
  printf("Hello\n");
  int *p = test();
	t();
  printf("The value in *p = %d\n", *p);
  printf("The address in p = %p\n", p);
  return 0;
}
