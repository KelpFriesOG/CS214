#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CHAR_SIZE 1

int main()
{

    char *str = malloc(CHAR_SIZE + 1);

    char input = NULL;

    input = getchar();

    int last_index = 0;

    str[last_index] = input;

    last_index++;

    int size = 1;

    while (input != '\n')
    {
        input = getchar();

        if (input == '\n')
        {
            last_index++;
            str[last_index] = '\0';
            printf("testinnn");
            continue;
        }

        char *np = realloc(str, size + 1);

        if (np == NULL)
        {

            printf("Could not allocate memory");
            free(str);
            break;
        }

        str[last_index] = input;
        str = np;
        last_index++;
        size++;
    }

    printf("\n%s\n", str);
    free(str);
}