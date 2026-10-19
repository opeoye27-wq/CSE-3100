#include <stdio.h>
#include <stdlib.h>

/* struct exam1{ */
/* 	int id; */
/* 	int points; */
/* }; */
/**/
/* typedef struct exam1 Exam1; */

typedef struct exam1 {
  int id;
  int points;
} Exam1;

int main() {
  /* struct exam1 student1; */
  Exam1 student1;

  student1.id = 1;
  student1.points = 87;

  printf("student1 id = %d\n", student1.id);
  printf("student1 points = %d\n", student1.points);

  /* struct exam1 students[10]; */
  Exam1 students[10];

  students[0].id = 123;
  students[0].points = 56;

  printf("students[0] id = %d\n", students[0].id);
  printf("students[0] points = %d\n", students[0].points);

  Exam1 *student2 = malloc(sizeof(Exam1));
  student2->id = 2;
  student2->points = 90;

  printf("student2 id = %d\n", student2->id);
  printf("student2 points = %d\n", student2->points);

  free(student2);
}
