#include <stdio.h>
#include "mymalloc.c"

int main(int argc, char *argv[])
{
    //     int *ptr = malloc(10);
    //     double *ptr2 = malloc(20);
    //     int *ptr3 = malloc(4016);
    //     printf("SIZE OF HEADER %d,\n", sizeof(HEADER));
    //     free(ptr);
    //     free(ptr3);
    //     free(ptr2);
    //     printf("TESTING");

    for (int i = 0; i < 120; i++)
    {
        char *ptr = malloc(1); // Allocate 1 byte of memory
        free(ptr);             // Release the memory
    }
}