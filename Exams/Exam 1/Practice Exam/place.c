#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_elements(char **elems, int count) {
  for (int i = 0; i < count; i++) {
    printf("%s\n", elems[i]);
  }
}

//counting lowercases
int count_lowercase(char *str) {
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            count++;
        }
    }
    return count;
}

int compare_string(const void * a, const void * b)
{
  //(char **)a converts a into a pointer to a string pointer.
  //*(char **)a dereferences it to get the actual string pointer.
  char *str1 = *(char **)a; 
  char *str2 = *(char **)b;

  int count1 = count_lowercase(str1);
  int count2 = count_lowercase(str2);

  return count1 - count2;

}

void qsort(void *base, size_t nel, size_t width, int compare_string())
{

}

int main(int argc, char *argv[]) 
{
  //argc = num command line args including prog name, 
  //argv = an array of strings containing those arguments
  //argv[0] = "./place" (the program name)
  //argv[1] = "abc", argv[2] = "abC", and so on.
  if (argc < 2) 
  {
    perror("Usage : ./place <strings>");
    exit(1);
  }
  qsort(&argv[1], argc - 1, sizeof(char *), compare_string); //add call to qsort 
  print_elements(&argv[1], argc - 1);

  return 0;
}