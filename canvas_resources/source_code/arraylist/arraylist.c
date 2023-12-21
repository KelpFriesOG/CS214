#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arraylist.h"
    // ensure that definitions are consistent with header

#ifndef DEBUG
#define DEBUG 0
#endif


// create storage and initialize length/size
// returns 1 on success, 0 for failure
int al_init(arraylist_t *L, unsigned capacity)
{
    L->data = (elem_t *) malloc(capacity * sizeof(elem_t));
    if (L->data == NULL) return 0;

    L->length = 0;
    L->size = capacity;

    return 1;
}

arraylist_t *al_create(unsigned capacity)
{
    arraylist_t *L = malloc(sizeof(arraylist_t));
    if (L == NULL) return NULL;

    if (al_init(L, capacity)) {
	return L;
    }

    free(L);
    return NULL;
}

void al_destroy(arraylist_t *L)
{
    free(L->data);
}

unsigned al_length(arraylist_t *L)
{
    return L->length;
}

// add specified element to end of list
// returns 1 on success, 0 on failure
// assumes list has been initialized
int al_push(arraylist_t *L, elem_t elem)
{
    // check whether array is full
    if (L->size == L->length) {
	// increase capacity
	int new_size = L->size * 2;

	elem_t *new_data = realloc(L->data, new_size * sizeof(elem_t));
	if (new_data == NULL) return 0;

	if (DEBUG) printf("Increased size to %d from %d\n", new_size, L->size);
	L->size = new_size;
	L->data = new_data;
    }

    L->data[L->length] = elem;
    L->length++;

    return 1;
}

// remove item from end of list
// write item to dest (if dest is non-NULL)
// return 1 on success, 0 on failure (i.e., list is empty)

int al_pop(arraylist_t *L, elem_t *dest)
{
    if (L->length == 0) return 0;

    L->length--;
    if (dest) *dest = L->data[L->length];
    if (DEBUG) printf("Removed %d; new length %d\n", L->data[L->length], L->length);

    return 1;
}
