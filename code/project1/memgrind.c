#include <stdio.h>
#include "mymalloc.c"

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

int main(int argc, char **argv)
{
    void *init_ptr = malloc(10);

    void *split_1 = malloc(16);

    free(split_1);
}
