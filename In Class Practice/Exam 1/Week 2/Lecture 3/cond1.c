// if a number is positive or negative or zero
//

#include <stdio.h>

int main() {
  int x = 0;

  if (x > 0) {
    printf("%d is positive\n", x);
  } else if (x < 0) {
    printf("%d is negative\n", x);
  } else {
    printf("%d is zero\n", x);
  }

  return 0;
}