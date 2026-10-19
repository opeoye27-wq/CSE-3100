#include <stdio.h>

typedef struct person {
  char name[10];
  int id;
  long phone;
} person;

int main() {
  person p1 = {"John", 123, 23456542345};
  printf("size of p1 %lu\n", sizeof(p1));
  printf("p1.name = %s\n",p1.name);
  printf("p1.id = %d\n",p1.id);
  printf("p1.phone = %ld\n",p1.phone);
  return 0;
}
