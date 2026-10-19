#include <stdio.h>

int main() {
  int a = 353;

	int *ip = &a;
	char *cp = &a;

	printf("ip  = %p\n",ip); //these will be the same
	printf("cp  = %p\n",cp); //these will be the same

	printf("*ip  = %d\n",*ip); //353
	printf("*cp  = %c\n",*cp); //a Reading as a character will be 1 byte
	printf("*cp  = %d\n",*cp); //97 Reading as an integer will be 4 bytes

  return 0;
}
