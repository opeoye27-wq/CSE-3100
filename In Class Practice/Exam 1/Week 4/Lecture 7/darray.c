#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *values;
  int size;
  int capacity;
} Integers;

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

#define get(n, idx) ((n).values[(idx)])

void init(Integers *n) {
  n->size = 0;
  n->capacity = INITIAL_CAPACITY;
  n->values = calloc(n->capacity, sizeof(int));
}

void append(Integers *n, int v) {
  if (n->size == n->capacity) {
    n->values = realloc(n->values, n->capacity * GROWTH_FACTOR * sizeof(int));
  }
  n->values[n->size++] = v;
}

void print_integers(Integers *n) {
  for (int i = 0; i < n->size; i++) {
    printf("%d\n", n->values[i]);
  }
}

void free_integers(Integers *n) {
  free(n->values);
  n->values = NULL;
  n->size = 0;
  n->capacity = INITIAL_CAPACITY;
}

int main() {
  Integers n;
  init(&n);
  for (int i = 0; i < 20; i++) {
    append(&n, i);
  }

  print_integers(&n);
  printf("%d\n", get(n, 5));
  free_integers(&n);
  return 0;
}