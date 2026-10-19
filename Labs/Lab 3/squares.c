#include <stdio.h>
#include <stdlib.h> //missing this for malloc and atoi

/* This program should print the sum of the elements
 *     1^2, 2^2, 3^2, ..., n^2
 * where n is an integer provided by the user on the
 * command line. Hunt down the bugs and squash them!
 * Seek out the memory leaks and plug them up! */

/* Computes the sum of the first n elements in the array. */
int sum(int n, int* arr)
{
    int i, sum = 0; //need to initialize sum before we can increment it
    for(i = 0; i < n; i++) //arrays contain arr[0] to arr[n - 1] elements so don't want to go all the way up to n
        sum += arr[i];
    
    return sum; //function needs to actually return sum
}

/* Fills the given array with the values
 * 1^2, 2^2, 3^2, ..., (n-1)^2, n^2. */
void fillSquares(int n, int* arr)
{
    int i;
    for(i = 0; i < n; i++) //indexes go up to n-1 in an array
        arr[i] = (i + 1) * (i + 1); //array indexes go from 0 to n-1 but numbers are shifted over 1 to n
}

/* Reads an integer n from arguments,
 * fills array with squares, and computes
 * the sum of the squares. Prints out the
 * sum before freeing all used memory. */
int main(int argc, char* argv[])
{
    int n, total;
    int* arr;

    // There must be at least 1 argument after the program
    // although only the first one is used.
    if(argc < 2) {
        printf("usage: ./squares n\n");
        return 1;
    }

    // convert the first argument from a string to a number
    n = atoi(argv[1]);
    if (n <= 0) {
        printf("n must be positive.");
        return 1;
    }
    //arr = (int*) malloc(n); this is supposed to allocate n bytes not n integers --> so if n = 4 need 4 x 4 = 16 bytes
    arr = malloc(n * sizeof(int)); 

    fillSquares(n, arr);
    total = sum(n, arr);
    printf("total: %d\n", total);
    free(arr); //need to free memory or else mem leak
    return 0;
}