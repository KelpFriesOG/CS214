#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.c"

void test1();
void test2();
void test3();
void test4();

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

    test1();
    printf("test1 complete");

    test2();
    printf("test2 complete");

    test3();
    printf("test3 complete");

    test4();
    printf("test4 complete");
}

void test1()
{
    for (int i = 0; i < 120; i++)
    {
        char *ptr = malloc(1); // Allocate 1 byte of memory
        free(ptr);             // Release the memory
    }
    // printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void test2()
{
    char *ptrArray[120]; // Array to store 120 pointers

    for (int i = 0; i < 120; i++)
    {
        ptrArray[i] = malloc(1); // Allocate 1 byte of memory and store the address
    }

    for (int i = 0; i < 120; i++)
    {
        free(ptrArray[i]); // Release the memory
    }

    // printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void test3()
{
    char *ptrArray[120];      // Array to store 120 pointers
    int allocated[120] = {0}; // Initialize the memory allocation status array
    int loc = 0;              // Current location

    for (int i = 0; i < 120; i++)
    {
        if (loc == 0 || (rand() % 2 == 0 && loc < 120))
        {
            // Allocate 1 byte of memory and store the address
            // printf("alloc loc=%d\n", loc);
            ptrArray[loc] = malloc(1);
            allocated[loc] = 1;
            loc++;
        }
        else
        {
            // Release the memory
            loc--;
            // printf("free loc=%d\n", loc);
            free(ptrArray[loc]);
            allocated[loc] = 0;
        }
    }

    printf("Process is done.\n");

    // Clean up any unreleased memory
    for (int i = 0; i < 120; i++)
    {
        if (allocated[i] == 1)
        {
            free(ptrArray[i]);
        }
    }

    // printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

// creates a pattern of 8 bytes of occupied data followed by free space of 8 bytes.
// free the data from left to right, forcing 2 sided coalesce for every free
void test4()
{

    // Use a for loop to allocate all 120 pointers (each memory space occupies 16 bytes total).
    char *list[120];
    for (int i = 0; i < 120; i++)
    {
        char *ptr = malloc(1); // Allocate 8 byte of memory
        list[i] = ptr;
    }

    // Using a for loop to free every odd numbered pointer in the ptr list.
    for (int i = 1; i < 120; i += 2)
    {
        free(list[i]);
    }

    // free all remaining data in between free blocks, forcing coalesce
    for (int i = 0; i < 120; i += 2)
    {
        free(list[i]);
    }
}

void test5()
{
}