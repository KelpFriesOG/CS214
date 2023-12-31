#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 512
#define MEMLENGTH_BYTES (MEMLENGTH * sizeof(double))
//#define HEADER_SIZE sizeof(HEADER)
#define HEADER_SIZE 8
#define ROUNDUP(x) (((x) + 7) & ~7)

static double memory[MEMLENGTH];

void coalesce()
{

    HEADER *current = (HEADER *)memory;

    if (current->size >= 4088)
    {
        return;
    }

    int begin_here = 0;

    // 80 actual, 56 expected for n byte
    /*
    int *ptr = malloc(10);
    double *ptr2 = malloc(20);
    int *ptr3 = malloc(4024);
    free(ptr);
    free(ptr3);
    free(ptr2);

    */

    // + HEADER_SIZE + current->size

    HEADER *next = (HEADER *)((char *)current + HEADER_SIZE + current->size);

    while ((char *)memory + begin_here + current->size != &memory[511] && (char *)memory + begin_here + current->size != &memory[510])
    {
        next = (HEADER *)((char *)current + HEADER_SIZE + current->size);

        if (next->is_occupied == 0 && current->is_occupied == 0)
        {
            // The line below is KEY, and works as intended.
            current->size = next->size + HEADER_SIZE + current->size;
            next = (HEADER *)((char *)current + HEADER_SIZE + current->size);
            continue;
        }

        begin_here += HEADER_SIZE + current->size;
        current = next;
    }
}

void *my_malloc(int size, int line, char *file)
{

    // BASE CASE: Cannot allocate 0 bytes
    if (size == 0)
    {
        return NULL;
    }

    size = ROUNDUP(size);

    // First ensure that the size given by the user is valid
    if (size > (MEMLENGTH * 8) - HEADER_SIZE)
    {
        return NULL;
    }

    // Then check if memory is uninitialized (all zeros)
    if (((HEADER *)memory)->size == 0)
    {
        HEADER *new = (HEADER *)memory;
        new->size = size;
        new->is_occupied = 1;

        // Then check if the memory is at least large enough to fit
        // another header with the minimum size of 8.
        // If so, then put another header there.
        if (size < MEMLENGTH_BYTES - 2 * HEADER_SIZE)
        {
            HEADER *next = (char *)new + HEADER_SIZE + size;
            next->size = MEMLENGTH_BYTES - 2 * HEADER_SIZE - size;
            next->is_occupied = 0;
        }

        printf("success \n");
        return memory + 1;
    }

    // Otherwise, the memory is already initialized,
    // so we use the first header to find the next,
    // and repeat until we find a header that is not
    // already occupied and has enough space for the
    // requested size.

    /*



    */


    else
    {
        HEADER *current = (HEADER *)memory;


        /*
        
        Need to check if the next header becomes the very end of the list
        or if it becomes a next header

        
        */
        HEADER *next;
        if(current->size != MEMLENGTH_BYTES - HEADER_SIZE)
        {
            HEADER *next = (char *)current + HEADER_SIZE + current->size;
        }else{
            HEADER *next = (char *)current + HEADER_SIZE;
        }
        int current_byte = 0;
        void *data_ptr = (void *)(current);

        // We traverse the array header by header.
        // We run this loop at least once, because
        // the array is guaranteed to have at least one header.
        do
        {
            
            data_ptr = (void *)((char *)current + HEADER_SIZE);

            //16 represents minimum space after data for header + free
            if (current->is_occupied == 0 && current->size - 16 >= size)
            {
                int sizeDiff = current->size - HEADER_SIZE - size;
                current->size = size;
                current->is_occupied = 1;
                next = (HEADER *)((char *)current + HEADER_SIZE + current->size);
                next->size = sizeDiff;
                next->is_occupied = 0;
                printf("success");
                return (char*)current + HEADER_SIZE;
                
            }


            current_byte += HEADER_SIZE + current->size;
            current = (HEADER *)((char *)current + HEADER_SIZE + current->size);


        } while (current_byte < MEMLENGTH_BYTES && (void *)current < (void *)memory + MEMLENGTH_BYTES);

        printf("ERROR: Not having enough space \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        
        return NULL;

        /*
        HEADER *head = (HEADER *)memory;
        HEADER *next = (char *)head + HEADER_SIZE + head->size;
        int n_byte = HEADER_SIZE + head->size;

        while (next->is_occupied == 1 || next->size < size)
        {

            n_byte += HEADER_SIZE + next->size;

            if (n_byte >= MEMLENGTH_BYTES)
            {

                // TODO: Print error message regarding not having enough space.
                // WIP: Only tested on when first header occupies ENTIRE ARRAY.
                printf("ERROR: Not having enough space \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
                return NULL;
            }

            next = (char *)next + HEADER_SIZE + next->size;
        }

        // If we find a header that is free and has
        // enough empty space. We then check if it
        // could have enough additional space to create
        // a meaningful free chunk after our alloted data.

        int old_size = next->size;
        next->size = size;

        if (old_size - (HEADER_SIZE + size) >= HEADER_SIZE)
        {
            ((HEADER *)((char *)(next + 1) + size))->size = old_size - (HEADER_SIZE + size);
            ((HEADER *)((char *)(next + 1) + size))->is_occupied = 0;
        }

        next->is_occupied = 1;

        printf("success \n");
        return next + 1;

        */
    }
}

/*

This function frees a chunk of memory. But also checks
the following conditions to make sure the pointer is valid:

If the pointer points to somewhere outside the array
then it is invalid.

If the pointer points to the middle of a chunk,
then it is invalid.

If the pointer points to an already freed chunk,
then it is invalid.

If pointer is valid, then it frees the associated
chunk of memory and does not return anything.
Otherwise it will print casewise error messages prior to
terminating.

*/
void my_free(void *ptr, int line, char *file)
{

    // Chack precondition: memory array has been initialized.
    // If it has not, then we print an error message and terminate.
    if ((HEADER *)memory == 0)
    {
        // TODO: Print error message regarding the memory not being initialized.
        // WIP: Untested.
        printf("ERROR: free() cannot be called before memory is initialized \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        return;
    }
    
    // First we check if the pointer is somewhere valid in the array.
    int in_array = 0;

    HEADER *holder;

    for (int i = 0; i < MEMLENGTH_BYTES; i++)
    {
        holder = (char *)(memory) + i;
        if (holder == ptr)
        {
            in_array = 1;
            break;
        }
    }

    // If not we print the appropriate error message
    // and terminate the function.
    if (in_array == 0)
    {
        // TODO: Print error message regarding the pointer being outside the memory.
        printf("ERROR: Pointer is outside the memory \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        return;
    }

    // Now we go through the array looking for the chunk
    // that the pointer points to.
    // If this search is unsuccessful, the pointer must
    // be in the array but also must be pointing
    // to the middle of a chunk.


/*
    // Adjusting ptr to point to its header
    HEADER *ptr_header = (HEADER *)((char *)ptr - HEADER_SIZE);

    // Now we go through the array looking for the chunk that the pointer points to.
    HEADER *current = (HEADER *)memory;
    void *data_ptr;  // pointer to the current data segment

    // We traverse the array header by header.
    while ((void *)current < (void *)memory + MEMLENGTH_BYTES)
    {
        data_ptr = (void *)((char *)current + HEADER_SIZE);  // data segment starts after the header

        // Check if we've found the correct memory block.
        if (data_ptr == ptr)
        {
            if (current->is_occupied == 0)
            {
                printf("ERROR: Pointer was already freed \n ORIGIN: at line %d in file %s", line, file);
                coalesce();
                return;
            }

            // Valid pointer found, free the memory block
            current->is_occupied = 0;
            coalesce();
            printf("success");
            return;
        }

        // Moving to the next block in the memory array
        current = (HEADER *)((char *)current + HEADER_SIZE + current->size);
    }
*/

    HEADER *current = (HEADER *)memory;
    int current_byte = 0;
    void *data_ptr = (void *)(current);

    // We traverse the array header by header.
    // We run this loop at least once, because
    // the array is guaranteed to have at least one header.
    do
    {
        
        data_ptr = (void *)((char *)current + HEADER_SIZE);

        
        if (current->is_occupied == 0 && data_ptr == ptr)
        {
            // TODO: Print error message regarding the pointer being freed already.
            printf("ERROR: Pointer is already freed \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
            coalesce();
            return;
        }

        else if (current->is_occupied == 1 && data_ptr == ptr)
        {
            // NOTE: In documentation make note of the fact that
            // we do not need to change any underlying data in the
            // process of calling free(), simply free().
            current->is_occupied = 0;
            coalesce();
            printf("success \n");
            return;
        }

        current_byte += HEADER_SIZE + current->size;
        current = (HEADER *)((char *)current + HEADER_SIZE + current->size);


    } while (current_byte < MEMLENGTH_BYTES && (void *)current < (void *)memory + MEMLENGTH_BYTES);

    // If we reach this point, we print the appropriate error message,
    // should be regarding the fact that the pointer DOES exist in
    // memory but does not point to the the beginning of a memory segment.

    printf("ERROR: Pointer does not point to the beginning of a memory segment \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
    return;
}


/* WIP: Function to make sure all bytes are free.*/
