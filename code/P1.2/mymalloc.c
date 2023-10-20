#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 512
#define MEMLENGTH_BYTES (MEMLENGTH * sizeof(double))
#define HEADER_SIZE 8
#define ROUNDUP(x) (((x) + 7) & ~7)

static double memory[MEMLENGTH];

void * initialized_memory_add(int size, int line, char *file, HEADER *current);
int free_valid_pointer(void *ptr, int line, char *file);

/**
 * Replication of malloc functions
 * @Kalpesh Chavan, Hieu Nguyen
*/



/** Coalesces adjacent free blocks of memory into a singular larger block
 * 
*/
void coalesce(){
    HEADER *current = (HEADER *)memory;

    if (current->size >= 4088){
        return;
    }

    int begin_here = 0;

    HEADER *next = (HEADER *)((char *)current + HEADER_SIZE + current->size);

    while ((char *)memory + begin_here + current->size != (char *)&memory[511] && (char *)memory + begin_here + current->size != (char *)&memory[510]){
        next = (HEADER *)((char *)current + HEADER_SIZE + current->size);

        if (next->is_occupied == 0 && current->is_occupied == 0){
            // The line below is KEY, and works as intended.
            current->size = next->size + HEADER_SIZE + current->size;
            next = (HEADER *)((char *)current + HEADER_SIZE + current->size);
            continue;
        }

        begin_here += HEADER_SIZE + current->size;
        current = next;
    }
}




/**inserts data blocks & headers into memory given enough space
 * 
*/
void *my_malloc(int size, int line, char *file){

    // BASE CASE: Cannot allocate 0 bytes
    if (size == 0){
        return NULL;
    }
    size = ROUNDUP(size);

    // First ensure that the size given by the user is valid
    if (size > (MEMLENGTH * 8) - HEADER_SIZE){
        return NULL;
    }

    // Then check if memory is uninitialized (all zeros)
    if (((HEADER *)memory)->size == 0){
        HEADER *new = (HEADER *)memory;
        new->size = size;
        new->is_occupied = 1;
        // Then check if the memory is at least large enough to fit
        // another header with the minimum size of 8.
        // If so, then put another header there.
        if (size < MEMLENGTH_BYTES - 2 * HEADER_SIZE)
        {
            HEADER *next = (HEADER *)((char *)new + HEADER_SIZE + size);
            next->size = MEMLENGTH_BYTES - 2 * HEADER_SIZE - size;
            next->is_occupied = 0;
        }
        return memory + 1;
    }
    // Otherwise, the memory is already initialized,
    // so we use the first header to find the next,
    // and repeat until we find a header that is not
    // already occupied and has enough space for the
    // requested size.
    else{
        //HEADER *current = ;
        HEADER *result = (HEADER *) initialized_memory_add(size, line, file, (HEADER *)memory);
        if(result != NULL){
            return result;
        }

        printf("ERROR: Not having enough space \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        return NULL;
    }
}

/**support class for my_malloc if memory is already initialized
 *
*/
void * initialized_memory_add(int size, int line, char *file, HEADER *current){
    
    HEADER *return_header;
    HEADER *next;
    if (current->size != MEMLENGTH_BYTES - HEADER_SIZE){
        next = (HEADER *)((char *)current + HEADER_SIZE + current->size);
    }
    else{
        next = (HEADER *)((char *)current + HEADER_SIZE);
    }
    int current_byte = 0;
    void *data_ptr = (void *)(current);

    // We traverse the array header by header.
    // We run this loop at least once, because
    // the array is guaranteed to have at least one header.
    do{

        data_ptr = (void *)((char *)current + HEADER_SIZE);

        // 16 represents minimum space after data for header + free
        if (current->is_occupied == 0 && current->size - 16 >= size){
            int sizeDiff = current->size - HEADER_SIZE - size;
            current->size = size;
            current->is_occupied = 1;
            next = (HEADER *)((char *)current + HEADER_SIZE + current->size);
            next->size = sizeDiff;
            next->is_occupied = 0;
            return_header = (HEADER *)((char *)current + HEADER_SIZE);
            return return_header;
        }

        current_byte += HEADER_SIZE + current->size;
        current = (HEADER *)((char *)current + HEADER_SIZE + current->size);

    } while (current_byte < MEMLENGTH_BYTES && (void *)current < (void *)memory + MEMLENGTH_BYTES);
    return NULL;
}


/*


*/

/**frees specific pointer in memory given pointer
 * 
 * This function frees a chunk of memory. But also checks
 * the following conditions to make sure the pointer is valid:

 * If the pointer points to somewhere outside the array
 * then it is invalid.

 * If the pointer points to the middle of a chunk,
 * then it is invalid.

 * If the pointer points to an already freed chunk,
 * then it is invalid.

 * If pointer is valid, then it frees the associated
 * chunk of memory and does not return anything.
 * Otherwise it will print casewise error messages prior to
 * terminating.
*/
void my_free(void *ptr, int line, char *file){

    int in_array = 0;
    char *running_pointer;

    // Chack precondition: memory array has been initialized.
    // If it has not, then we print an error message and terminate.
    if ((HEADER *)memory == 0){
        // TODO: Print error message regarding the memory not being initialized.
        // WIP: Untested.
        printf("ERROR: free() cannot be called before memory is initialized \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        return;
    }

    // First we check if the pointer is somewhere valid in the array.
    for (int i = 0; i < MEMLENGTH_BYTES; i++){
        running_pointer = (char *)(memory) + i;
        if (running_pointer == ptr){
            in_array = 1;
            break;
        }
    }

    // If not we print the appropriate error message
    // and terminate the function.
    if (in_array == 0){        
        printf("ERROR: Pointer is outside the memory \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        return;
    }

    // Now we go through the array looking for the chunk
    // that the pointer points to.
    // If this search is unsuccessful, the pointer must
    // be in the array but also must be pointing
    // to the middle of a chunk.
    int successful_free = free_valid_pointer(ptr, line, file);
    if(successful_free == 1){
        return;
    }

    // If we reach this point, we print the appropriate error message,
    // should be regarding the fact that the pointer DOES exist in
    // memory but does not point to the the beginning of a memory segment.
    printf("ERROR: Pointer does not point to the beginning of a memory segment \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
    return;
}


/** support function for my_free() if the memory is initialized and pointer is in array
 * 
*/
int free_valid_pointer(void *ptr, int line, char *file){

    HEADER *current = (HEADER *)memory;
    int current_byte = 0;
    void *data_ptr = (void *)(current);

    // We traverse the array header by header.
    // We run this loop at least once, because
    // the array is guaranteed to have at least one header.
    do{

        data_ptr = (void *)((char *)current + HEADER_SIZE);

        if (current->is_occupied == 0 && data_ptr == ptr){
            // TODO: Print error message regarding the pointer being freed already.
            printf("ERROR: Pointer is already freed \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
            coalesce();
            return 0;
        }

        else if (current->is_occupied == 1 && data_ptr == ptr){
            // NOTE: In documentation make note of the fact that
            // we do not need to change any underlying data in the
            // process of calling free(), simply free().
            current->is_occupied = 0;
            coalesce();
            return 1;
        }

        current_byte += HEADER_SIZE + current->size;
        current = (HEADER *)((char *)current + HEADER_SIZE + current->size);

    } while (current_byte < MEMLENGTH_BYTES && (void *)current < (void *)memory + MEMLENGTH_BYTES);

    return -1;

}

/* WIP: Function to make sure all bytes are free.*/


/**checks that entirety of memory is empty
 * 
*/
int memory_is_empty()
{

    // If the first header points to a fully coalesced block of memory
    if (((HEADER *)memory)->is_occupied == 0 && ((HEADER *)memory)->size == 4088)
    {
        return 1;
    }

    // Else if the first header has a size of 0 (indicating unitialized memory)
    // BY DEFINITION: UNINITIALIZED MEMORY IS CONSIDERED EMPTY
    else if (((HEADER *)memory)->size == 0)
    {
        return 1;
    }

    // Otherwise memory must be partially occupied
    else
    {
        return 0;
    }
}
