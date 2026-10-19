#include <stdio.h>

int main() {
    int arr[5];  // Uninitialized local array

    // Print the values in the array
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);  // Expected to print garbage values, but could be 0 in some environments
    }

    return 0;
}
