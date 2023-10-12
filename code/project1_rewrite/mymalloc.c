#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 512
#define MEMLENGTH_BYTES (MEMLENGTH * sizeof(double))
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

    int n_byte = 0;

    while (n_byte + current->size < 4096)
    {

        HEADER *next = (HEADER *)((char *)current + HEADER_SIZE + current->size);

        if (current->is_occupied == 0 && next->is_occupied == 0)
        {
            current->size = current->size + 8 + next->size;
            next = (char *)current + HEADER_SIZE + current->size;
            n_byte += n_byte + HEADER_SIZE + current->size;
            continue;
        }

        n_byte += n_byte + HEADER_SIZE + current->size;
        current = next;
    }
}

void *my_malloc(size_t size, int line, char *file)
{

    // BASE CASE: Cannot allocate 0 bytes
    if (size == 0)
    {
        return NULL;
    }

    size = ROUNDUP(size);

    // First ensure that the size given by the user is valid
    if (size > (MEMLENGTH * 8) - 8)
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

        return memory + 1;
    }

    // Otherwise, the memory is already initialized,
    // so we use the first header to find the next,
    // and repeat until we find a header that is not
    // already occupied and has enough space for the
    // requested size.
    else
    {
        HEADER *head = (HEADER *)memory;
        HEADER *next = (char *)head + HEADER_SIZE + head->size;

        while (next->is_occupied == 1 || next->size < size)
        {
            next = (char *)next + next->size;
        }

        // If we find a header that is free and has
        // enough empty space. We then check if it
        // could have enough additional space to create
        // a meaningful free chunk after our alloted data.

        int old_size = next->size;
        next->size = size;

        if (old_size - (8 + size) >= 16)
        {
            ((HEADER *)((char *)(next + 1) + size))->size = old_size - (8 + size);
            ((HEADER *)((char *)(next + 1) + size))->is_occupied = 0;
        }

        next->is_occupied = 1;

        return (char *)next + HEADER_SIZE;
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
        print("ERROR: free() cannot be called before memory is initialized \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
        return;
    }

    // First we check if the pointer is somewhere valid in the array.
    int in_array = 0;

    for (int i = 0; i < MEMLENGTH; i++)
    {
        if ((char *)(memory) + i == ptr)
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

    HEADER *current = (HEADER *)memory;
    int current_byte = 0;

    // We traverse the array header by header.
    // We run this loop at least once, because
    // the array is guaranteed to have at least one header.
    do
    {

        void *data_ptr = (void *)(current + 1);

        if (current->is_occupied == 0 && data_ptr == ptr)
        {
            // TODO: Print error message regarding the pointer being freed already.
            printf("ERROR: Pointer is already freed \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
            coalesce();
            return;
        }

        else if (current->is_occupied == 1 && data_ptr == ptr)
        {
            current->is_occupied = 0;
            coalesce();
            return;
        }

        current_byte += HEADER_SIZE + current->size;
        current = (HEADER *)((char *)current + HEADER_SIZE + current->size);

    } while (current_byte < MEMLENGTH);

    // If we reach this point, we print the appropriate error message,
    // should be regarding the fact that the pointer DOES exist in
    // memory but does not point to the the beginning of a memory segment.

    printf("ERROR: Pointer does not point to the beginning of a memory segment \n ORIGIN: at line %d in file %s", __LINE__, __FILE__);
    return;
}
