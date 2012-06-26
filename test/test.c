#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void my_getline()
{
    int buf_size = 3;
    char *result;
    result = malloc(sizeof(*result));
    strcpy(result, "");

    char buffer[buf_size];

    fprintf(stdout, "Result length: %d\n", (int)strlen(result));
    fprintf(stdout, "Buffer length: %d\n", (int)strlen(buffer));
    do
    {
        memset(buffer, 0, buf_size);

        printf("Enter something: ");
        if (fgets(buffer, buf_size, stdin) == NULL)
        {
            fprintf(stdout, "Error reading input.\n");
            return;
        }

        result = realloc(result, strlen(result) + strlen(buffer));
        strcat(result, buffer);

        fprintf(stdout, "You entered: %s\n", buffer);
        fprintf(stdout, "The result: %s\n", result);
        fprintf(stdout, "Result length: %d\n", (int)strlen(result));
        fprintf(stdout, "Buffer length: %d\n", (int)strlen(buffer));

    }
    while (strstr(buffer, "\n") == NULL);
    while (1);

    free(result);
}

int main(int argc, char **argv)
{
    my_getline();

    return 0;
}
