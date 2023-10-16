#include <stdio.h>
#include "mymalloc.c"

int main(int argc, char *argv[])
{



    int *ptr = malloc(10);
    double *ptr2 = malloc(20);/*
    int *ptr3 = malloc(4016);

    free(ptr);
    free(ptr);
    free(ptr3);
    free(ptr2);
*/
    for(int i = 0; i < 120; i++) {
        char *ptr = malloc(1);  // Allocate 1 byte of memory
        free(ptr);  // Release the memory
    }
}


void test1() {
    
//    printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}