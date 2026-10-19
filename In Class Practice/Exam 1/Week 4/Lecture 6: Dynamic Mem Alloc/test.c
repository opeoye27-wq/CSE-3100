#include <stdio.h>

int *test() {
  int a = 234;
  return &a;
}

int main(int argc, char **argv) {
  printf("Hello\n");
  int *p = test();
  printf("The value in *p = %d\n", *p);
  return 0;
}

