#include "mymalloc.h"
#include <stdio.h>
#include <string.h>

// Requirements based on 1.3
#define MEMLENGTH 512
static double memory[MEMLENGTH];

// Now for malloc to work we need to visualize
// the array being empty at first...

// Then how to we know what part of the array
// is "mallocable"?

// Remember that if this is a linked list,
// then there should be no gaps between elements.
// This means that the first empty space
// is always at the end of the linked list!

// HELPER FUNCTIONS

/*Returns a positive number (1 more the index where the match was found)
if the pointer points to some address in memory array,
and returns 0 otherwise.*/
int points_to_memory(void *ptr)
{
    for (int i = 0; i < 512; ++i)
        if (ptr == &memory[i])
            return i + 1;

    return 0;
}

// NOTE: malloc will allocate memory from the storage array: memory
void *mymalloc(unsigned len, char *file, int line)
{

    /* If the user puts in a number greater than
    504, then wen cannot fit the metadata AND the
    space the user requested regardless of the state
    of the array.*/
    if (len == 0 || len > 504)
    {
        return NULL;
    }

    // Rounding up the number the user put in
    // to the nearest multiple of 8.
    if (len % 8 != 0)
    {
        len = len + (8 - (len % 8));
    }

    // WIP
    // Checking to see if the array is empty.
    // There may be edge cases where this fails.
    int is_empty = 1;
    for (int i = 0; i < 512; i++)
    {
        if (memory[i] != 0)
        {
            is_empty = 0;
            break;
        }
    }

    // If the array is empty then
    // we initialize by splitting the
    // empty array into a occupied
    // section followed by the
    // free section.
    // (NOTE THAT WITH THE size
    // of the free section may be 0
    // but this is fine for now)
    if (is_empty)
    {
        header occupied = {1, len};
        header free = {0, 512 - (len + 16)};
        memcpy(memory, &occupied, 8);
        memcpy((char *)(memory + 1) + len, &free, 8);
        return memory + 1;
    }

    header *current = (header *)memory;
    unsigned nth_byte = 0;

    while (current->is_occupied)
    {
        nth_byte += (8 + current->size);
        current = (header *)((char *)(current + 1) + current->size);
    };

    // If there is enough free space in
    // current to occupy another minimum sized
    // chunk (16 bytes or more),
    // then we do that!
    if (len + 16 < current->size)
    {
        int old_size = current->size;
        current->size = len;
        header next = {0, old_size - len - 8};
        memcpy((char *)(current + 1) + current->size, &next, 8);
    }

    return current + 1;
}

// NOTE: free will deallocate memory from the storage array: memory
void myfree(void *ptr, char *file, int line)
{
    int in_memory = points_to_memory(ptr);
    if (!in_memory)
    {
        return;
    }
    else
    {
        ((header *)ptr - 1)->is_occupied = 0;

        header *current = ((header *)ptr - 1);

        int og_size = current->size;

        int nth_byte = 8 * (in_memory - 1) + current->size;
        current = (header *)((char *)(current + 1) + current->size);

        int adj_free_bytes = 0;

        while (!current->is_occupied && nth_byte < 511)
        {
            adj_free_bytes += (8 + current->size);
            nth_byte += (8 + current->size);
            current = (header *)((char *)(current + 1) + current->size);
        }

        current = ((header *)ptr - 1);
        current->size = og_size + adj_free_bytes;

        printf("%d", current->size);
    }
}
