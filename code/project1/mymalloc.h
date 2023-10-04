#include <stdio.h>

typedef struct // Always occupies 8 bytes total.
{

    int is_free; // occupies 4 bytes I think.
    size_t size; // occupies 4 bytes I think.

} header;

// typedef struct chunk // At least occupies 16 bytes
// {

//     header metadata = {0, 8};
//     // occupies 8 bytes.
//     double payload[metadata.size];
//     // occupies size bytes, where size is non-zero a multiple of 8.

// } chunk;

// malloc takes in an unsigned int
// (0 or positive int)
// that tells it how many bytes to allocate
void *malloc(unsigned);

// free will take in a pointer created by malloc
// and free the memory associated with it!
void free(void *);
