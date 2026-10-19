#include <stdio.h>

// Function that accepts an array (as a pointer) and its size as parameters
void printArray(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    printf("arr[%d] = %d\n", i, arr[i]);
  }
  arr[0] = 11;
}

int main() {
  int arr[] = {10, 20, 30, 40, 50};        // Array of size 5
  int size = sizeof(arr) / sizeof(arr[0]); // Calculate the size of the array

  // Pass the array and its size to the function
  printArray(arr, size);

  printf("arr[0] in main after returning from function :%d\n", arr[0]);
  return 0;
}