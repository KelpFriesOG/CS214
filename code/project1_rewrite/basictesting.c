#include <stdio.h>
#include "mymalloc.c"

int main(int argc, char *argv[])
{
    int *ptr = malloc(10);
    double *ptr2 = malloc(20);
    int *ptr3 = malloc(4032);
    free(ptr);
    free(ptr3);
    free(ptr2);
}