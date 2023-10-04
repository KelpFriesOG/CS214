#include "mymalloc.h"

// Requirements based on 1.3
#define MEMLENGTH 512
static double memory[MEMLENGTH];

int main()
{

    // Requirements based on 1.3
    char *heapstart = (char *)memory;
    char *byte200 = heapstart + 200;
}

// Now for malloc to work we need to visualize
// the array being empty at first...

// Then how to we know what part of the array
// is "mallocable"?

// Remember that if this is a linked list,
// then there should be no gaps between elements.
// This means that the first empty space
// is always at the end of the linked list!

// NOTE: malloc will allocate memory from the storage array: memory
void *malloc(unsigned len)
{

    // Base Case 1: len = 0
    if (len == 0)
    {
        return NULL;
    }

    // If the requested amount of bytes
    // is not a multiple of 8, then round up!
    if (len % 8 != 0)
    {
        len = len + (len % 8);
    }

    // Otherwise begin setup to iterate through
    // memory.

    header *curr = memory;
    header *prev = NULL;

    while (curr != NULL && !curr->is_free)
    {
        prev = curr;
        curr = (header *)((char *)curr + 8 + curr->size);
    }

    if(curr->size < len){
        if(curr + 1 + curr->size == NULL){

        }
    }

    curr->size = len;
    curr->is_free = 0;

    return curr + 1;

}

// NOTE: free will deallocate memory from the storage array: memory
void free(void *ptr)
{

}
