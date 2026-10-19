#include <stdio.h>

int main() {
  int x = 5;

  while (x > 0) {
    printf("countdown %d\n", x);
    if (x % 2 == 0)
      break;
    x--;
  }
  return 0;
}