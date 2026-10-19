#include <stdio.h>

int main() {
  int x = 1;

  switch (x) {
  case 1:
    printf("Mon\n");
    break; //if you don't use break, it will go to the next case and print all the cases after it until it finds a break or the end of the switch statement.

  case 2:
    printf("Tue\n");
    break;

  case 3:
    printf("wed\n");
    break;

  case 4:
    printf("Thu\n");
    break;

  case 5:
    printf("Fri\n");
    break;

  case 6:
    printf("sat\n");
    break;
  case 7:
    printf("sun\n");
    break;
  default:
    printf("Wrong option\n");
  }
}