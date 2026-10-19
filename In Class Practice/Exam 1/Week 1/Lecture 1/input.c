/* task 1 - take an integer as an input */
/* task 2  - multiply the integer with 2  */
/* task 3 - print the final result */

#include <stdio.h>

int main() {
  int n;
  printf("Enter an Integer:");
  scanf("%d", &n);

  printf("Your input is: %d\n", n);

  int result = n * 2;

  printf("The final result is: %d\n", result);

  printf("after multiplying %d with 2, the final result is %d\n", n, result);

  return 0;
}