// find if a number is even or odd
//
#include <stdio.h>

int main() {
  int x = 6;

  if (x % 2 == 0)
    printf("%d is even\n", x);
	
  else{
    printf("%d is odd\n", x);
		printf("This is in else\n");	
	}

	return 0;
}