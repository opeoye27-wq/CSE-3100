#include <stdio.h>

int main() {
  int guy = 10;

  int *knows_a_guy = &guy;

  int **knows_knows_a_guy = &knows_a_guy;

  printf("guy = %d\n", guy);
  printf("&guy = %p\n", &guy);

  printf("knows_a_guy = %p\n", knows_a_guy);
  printf("*knows_a_guy = %d\n", *knows_a_guy);
  printf("&knows_a_guy = %p\n", &knows_a_guy);

  printf("knows_knows_a_guy = %p\n", knows_knows_a_guy);
  printf("*knows_knows_a_guy = %p\n", *knows_knows_a_guy);
  printf("**knows_knows_a_guy = %d\n", **knows_knows_a_guy);
  printf("&knows_knows_a_guy = %p\n",
         &knows_knows_a_guy); // how to store this value?

  return 0;
}