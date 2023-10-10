#include <stdio.h>
#include "mymalloc.c"

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#ifndef TESTCASE
#define TESTCASE 0
#endif

int main(int argc, char **argv)
{

    void *init_ptr = malloc(4072);

    // void *split_1 = malloc(16);

    // free(split_1);

    free(init_ptr);
}
