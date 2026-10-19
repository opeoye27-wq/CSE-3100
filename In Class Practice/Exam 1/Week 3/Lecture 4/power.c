// task  - create a function that computes power of a number
//
// it should return  integer and take two integers as parameters

#include <stdio.h>

int power(int base, int n) {
  // base = 2 ^ n = 3

  int result = 1;

  while (n > 0) {
    result = result * base;
    n--;
  }

  return result;
}

int power_rec(int base, int n) {
  if (n == 0) {
    return 1;
  } else {
    return base * power_rec(base, n - 1);
  }
}

// 2^3
// 2*power_rec(2,2)
// 2*2*power_rec(2,1)
// 2*2*2*power_rec(2,0)
// 2*2*2*1 = 8

int main() {

  printf("2^3 = %d\n", power(2, 3));
  printf("2^3 = %d\n", power_rec(2, 3));

  return 0;
}