#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.c"
#include <sys/time.h>

void test1();
void test2();
void test3();
void test4();
void test_looper(int test_number);
struct timeval start_time, end_time;

/**testing functions for mymalloc.c
 * @Kalpesh Chavan, Hieu Nguyen
 */

/**starts function testing
 *
 */
int main(int argc, char *argv[])
{

    test_looper(1);
    // printf("memory is empty: %d \n", memory_is_empty());
    test_looper(2);
    // printf("memory is empty: %d \n", memory_is_empty());
    test_looper(3);
    // printf("memory is empty: %d \n", memory_is_empty());
    test_looper(4);
    // printf("memory is empty: %d \n", memory_is_empty());
    test_looper(5);
    // printf("memory is empty: %d \n", memory_is_empty());
}

/**takes an int representing the test number, and loops the test 50 times
 *
 */
void test_looper(int test_number)
{

    double total_time;

    for (int i = 0; i < 50; i++)
    {
        gettimeofday(&start_time, NULL);
        switch (test_number)
        {
        case 1:
            test1();
            break;

        case 2:
            test2();
            break;

        case 3:
            test3();
            break;

        case 4:
            test4();
            break;

        case 5:
            break;

        default:
            break;
            return;
        }

        gettimeofday(&end_time, NULL);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000.0 + (end_time.tv_usec - start_time.tv_usec);
        printf("time take for run (in microseconds): %f \n", time_taken);
        total_time += time_taken;
    }
    printf("test%d complete, avereage time (in microseconds): %f \n", test_number, (total_time / 50));
}

/**mallocs a pointer and immediately frees it 120 times
 *
 */
void test1()
{
    for (int i = 0; i < 120; i++)
    {
        char *ptr = malloc(1);
        free(ptr);
    }
}

/**mallocs 120 blocks and then frees all of them
 *
 */
void test2()
{
    char *ptrArray[120];

    for (int i = 0; i < 120; i++)
    {
        ptrArray[i] = malloc(1);
    }

    for (int i = 0; i < 120; i++)
    {
        free(ptrArray[i]);
    }
}

/**makes array of 120 pointers, randomly allocates or frees pointers until 120 are allocated
 *
 */
void test3()
{
    char *ptrArray[120];
    int allocated[120] = {0};
    int loc = 0;

    for (int i = 0; i < 120; i++)
    {
        if (loc == 0 || (rand() % 2 == 0 && loc < 120))
        {
            ptrArray[loc] = malloc(1);
            allocated[loc] = 1;
            loc++;
        }
        else
        {
            loc--;
            free(ptrArray[loc]);
            allocated[loc] = 0;
        }
    }
    for (int i = 0; i < 120; i++)
    {
        if (allocated[i] == 1)
        {
            free(ptrArray[i]);
        }
    }
}

/**creates a pattern of data and free blocks, then frees data to constantly coalesce frees
 *
 */
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
    // printf("MemClear?: %d\n", memory_is_empty());  // Check if memory is cleared
}

/* allocates a sequence of randomly sized (from 8 to 128 bytes sized) pointers
 * and then frees them once malloc fails. (FAILURE IS INTENTIONAL once per run)
 */
