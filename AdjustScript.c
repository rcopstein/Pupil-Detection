#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {

    int count[4];

    count[0] = 0;
    count[1] = 0;
    count[2] = 0;
    count[3] = 0;

    char* line = NULL;
    size_t len = 0;

    FILE* file = fopen("./Result_Video5.txt", "r");

    while (getline(&line, &len, file) != -1)
    {
        if (strcmp(line, "0\n") == 0) { count[0]++; }
        if (strcmp(line, "1\n") == 0) { count[1]++; }
        if (strcmp(line, "2\n") == 0) { count[2]++; }
        if (strcmp(line, "3\n") == 0) { count[3]++; }
    }

    printf("%d %d %d %d", count[0], count[1], count[2], count[3]);

    FILE* file2 = fopen("./R5.txt", "w+");
    fprintf(file2, "%d %d %d %d", count[0], count[1], count[2], count[3]);

}