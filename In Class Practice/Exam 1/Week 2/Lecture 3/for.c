#include <stdio.h>

int main() {

  int sum = 0;

  for (int i = 1; i <= 5; i++) {
    sum = sum + i;
  }


	printf("Sum of 1 to 5 is %d\n",sum);
  return 0;
}