#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.c"
#include <sys/time.h>

void test1();
void test2();
void test3();
void test4();
struct timeval start_time, end_time;


/**testing functions for mymalloc.c
 * @Kalpesh Chavan, Hieu Nguyen
*/

/**starts function testing
 * 
*/
int main(int argc, char *argv[]){
    
    double total_time;
    
    for(int i = 0; i < 50; i++){
        gettimeofday(&start_time, NULL);
        test1();
        gettimeofday(&end_time, NULL);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000.0
                          + (end_time.tv_usec - start_time.tv_usec);
        printf("time take for run (in microseconds): %f \n", time_taken);
        total_time += time_taken;
    }
    printf("test1 complete, avereage time (in microseconds): %f \n", (total_time/50));
    total_time = 0;


    for(int i = 0; i < 50; i++){
        gettimeofday(&start_time, NULL);
        test2();
        gettimeofday(&end_time, NULL);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000.0
                          + (end_time.tv_usec - start_time.tv_usec);
        printf("time take for run (in microseconds): %f \n", time_taken);
        total_time += time_taken;
    }
    printf("test2 complete, avereage time (in microseconds): %f \n", (total_time/50));
    total_time = 0;


    for(int i = 0; i < 50; i++){
        gettimeofday(&start_time, NULL);
        test3();
        gettimeofday(&end_time, NULL);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000.0
                          + (end_time.tv_usec - start_time.tv_usec);
        printf("time take for run (in microseconds): %f \n", time_taken);
        total_time += time_taken;
    }
    printf("test3 complete, avereage time (in microseconds): %f \n", (total_time/50));
    total_time = 0;



    for(int i = 0; i < 50; i++){
        gettimeofday(&start_time, NULL);
        test4();
        gettimeofday(&end_time, NULL);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000.0
                          + (end_time.tv_usec - start_time.tv_usec);
        printf("time take for run (in microseconds): %f \n", time_taken);
        total_time += time_taken;
    }
    printf("test4 complete, avereage time (in microseconds): %f \n", (total_time/50));
    total_time = 0;



    for(int i = 0; i < 50; i++){
        gettimeofday(&start_time, NULL);
        test5();
        gettimeofday(&end_time, NULL);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1000000.0
                          + (end_time.tv_usec - start_time.tv_usec);
        printf("time take for run (in microseconds): %f \n", time_taken);
        total_time += time_taken;
    }
    printf("test5 complete, avereage time (in microseconds): %f \n", (total_time/50));
    total_time = 0;

    
}

/**mallocs a pointer and immediately frees it 120 times 
 * 
*/
void test1(){
    for (int i = 0; i < 120; i++)
    {
        char *ptr = malloc(1); // Allocate 1 byte of memory
        free(ptr);             // Release the memory
    }
    //printf("MemClear?: %d\n", memory_is_empty());  // Check if memory is cleared
}

/**mallocs 120 blocks and then frees all of them
 * 
*/
void test2(){
    char *ptrArray[120]; // Array to store 120 pointers

    for (int i = 0; i < 120; i++)
    {
        ptrArray[i] = malloc(1); // Allocate 1 byte of memory and store the address
    }

    for (int i = 0; i < 120; i++)
    {
        free(ptrArray[i]); // Release the memory
    }

    //printf("MemClear?: %d\n", memory_is_empty());  // Check if memory is cleared
}


/**makes array of 120 pointers, randomly allocates or frees pointers until 120 are allocated
 * 
*/
void test3(){
    char *ptrArray[120];      // Array to store 120 pointers
    int allocated[120] = {0}; // Initialize the memory allocation status array
    int loc = 0;              // Current location

    for (int i = 0; i < 120; i++){
        if (loc == 0 || (rand() % 2 == 0 && loc < 120)){
            // Allocate 1 byte of memory and store the address
            // printf("alloc loc=%d\n", loc);
            ptrArray[loc] = malloc(1);
            allocated[loc] = 1;
            loc++;
        }
        else{
            // Release the memory
            loc--;
            // printf("free loc=%d\n", loc);
            free(ptrArray[loc]);
            allocated[loc] = 0;
            i -= 2;
        }
    }

    //printf("Process is done.\n");

    // Clean up any unreleased memory
    for (int i = 0; i < 120; i++)
    {
        if (allocated[i] == 1)
        {
            free(ptrArray[i]);
        }
    }

    //printf("MemClear?: %d\n", memory_is_empty());  // Check if memory is cleared
}



/**creates a pattern of data and free blocks, then frees data to constantly coalesce frees
 * 
*/
void test4(){

    // Use a for loop to allocate all 120 pointers (each memory space occupies 16 bytes total).
    char *list[120];
    for (int i = 0; i < 120; i++){
        char *ptr = malloc(1); // Allocate 8 byte of memory
        list[i] = ptr;
    }

    // Using a for loop to free every odd numbered pointer in the ptr list.
    for (int i = 1; i < 120; i += 2){
        free(list[i]);
    }

    // free all remaining data in between free blocks, forcing coalesce
    for (int i = 0; i < 120; i += 2){
        free(list[i]);
    }
    //printf("MemClear?: %d\n", memory_is_empty());  // Check if memory is cleared
}

void test5(){


}