#include <stdio.h>

int main() {
    /* FILE *fout = fopen("stu.txt", "w"); */
    /* if (fout == NULL) { */
    /*     perror("fopen"); */
    /*     return 1; */
    /* } */
    /**/
    /* fprintf(fout, "%s %d %.3f\n", "Alice", 30, 95.5); */
    /* fprintf(fout, "%s %d %.3f\n", "Bob",   25, 87.3); */
    /* fclose(fout); */

    FILE *fin = fopen("stu.txt", "r");
    if (fin == NULL) {
        perror("fopen");
        return 1;
    }

    char name[50];
    int age;
    float score;

    printf("%-10s %-5s %-6s\n", "Name", "Age", "Score");
    printf("-------------------------\n");

    while (fscanf(fin, "%s %d %f", name, &age, &score) == 3) {
        printf("%-10s %-5d %-6.2f\n", name, age, score);
    }

    fclose(fin);
    return 0;
}

