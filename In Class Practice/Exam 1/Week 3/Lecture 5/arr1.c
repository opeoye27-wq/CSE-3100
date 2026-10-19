#include <stdio.h>

void print_array(int arr[], int n) {
  for (int i = 0; i < n; i++) {
    printf("arr[%d] = %d\n", i, arr[i]);
  }
}

int main() {
  int n;
  printf("Enter the size of array:");
  scanf("%d", &n);

  int arr[n];
  for (int i = 0; i < n; i++) {
    printf("Enter arr[%d]:", i);
    scanf("%d", &arr[i]);
  }
	print_array(arr,n);
}