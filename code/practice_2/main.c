#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CHAR_SIZE 1

int main()
{

    char *str = malloc(CHAR_SIZE + 1);

    char input = 'a';

    int last_index = 1;

    int size = 2;

    while (input != '\n')
    {
        input = getchar();

        if (input != '\n')
        {
            str[last_index - 1] = input;
            char *new_str = realloc(str, size + 1);
            str = new_str;
            // free(new_str);
            size++;
            last_index++;
        }

        else
        {

            str[last_index] = '\0';
        }
    }

    printf("\n%s\n", str);

    printf("\nCharacter by character (size = %d)\n", size);

    for(int i = 0; i < size; i++){

        printf("%d: %x\n", i, str[i]);
        
    }

    printf("ENDED\n");

    free(str);
}