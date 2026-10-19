#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s)
{
    perror(s);
    exit(1);
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(char *s1, char *s2)
{
    // TODO 
    char *result;
    result = malloc(strlen(s1) + strlen(s2) + 1); //need the extra 1 space for '\0'
    if(result == NULL) //if malloc fails print an error message
        my_error("malloc");

    int i;
    for(i = 0; s1[i] != '\0'; i++) //adding s1
    {
        result[i] = s1[i];
    }
    result[i] = '\0';
    
    int j;
    for(j = 0; s2[j] != '\0'; j ++) //adding s2 now
    {
        result[i] = s2[j]; //pick up where s1 left off
        i ++;
    }
    result[i] = '\0';

    return result;

}

int main(int argc, char *argv[])
{
    char    *s;

    s = my_strcat("", argv[0]);

    for (int i = 1; i < argc; i ++) {
        char *old_s = s;
        s = my_strcat(s, argv[i]);
        free(old_s); //to fix mem leak
    }

    printf("%s\n", s);
    free(s); //need to free mem when done with it

    return 0;
}