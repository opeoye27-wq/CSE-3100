#include <stdio.h>

struct student {
  char name[10];
  int id;
};

void boo(struct student s) { printf("s.name = > %s\n", s.name); }
void foo(struct student *s) {
  printf("s.name = > %s\n", s->name);
  s->id++;
}

int main() {
  struct student s1 = {"Bob", 12};
  struct student s2 = {"Alice", 11};
  struct student s3 = {"johb", 21};
  printf("s1.name = > %s\n", s1.name);
  printf("s1.id = > %d\n", s1.id);

  struct student myclass[3] = {{"Bob", 12}, {"Alice", 13}, {"John", 13}};
  boo(s1);
  foo(&s1);

  printf("s1.id = > %d\n", s1.id);
}