#include <stdio.h>
#include "mymalloc.c"

int main(int argc, char *argv[])
{





    int *ptr = malloc(10);
    double *ptr2 = malloc(20);
    int *ptr3 = malloc(4016);

    free(ptr);
    free(ptr);
    free(ptr3);
    free(ptr2);

   
    
}


void test1() {
     for(int i = 0; i < 120; i++) {
        char *ptr = malloc(1);

        //something wrong here, when it goes to free, it changes from position to value
        //
        free(ptr);
    }
//    printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}