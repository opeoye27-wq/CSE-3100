#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *fp;
  char name[50];
  int age;
  float gpa;

  fp = fopen("students.txt", "r");
  if (fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  while (fscanf(fp, "%49s %d %f", name, &age, &gpa) == 3) {
    printf("Name: %s, Age: %d, GPA: %.2f\n", name, age, gpa);
  }

  fclose(fp);
  return 0;
}