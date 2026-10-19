#include <stdio.h>

int b = 100; // global variable

void test() {
  static int i = 1; // a static variable
  i = i + 1;
  printf("i in test = %d\n", i);
  printf("b in test = %d\n", b);
}

int main() {
  int a = 11; // a local variable
  printf("a in main = %d\n", a);
  b++;
  printf("b in main = %d\n", b);
  test();
  test();
  test();
  return 0;
}