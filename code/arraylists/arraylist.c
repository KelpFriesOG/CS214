#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arraylist.h"

#ifndef DEBUG
#define DEBUG 0
#endif

// What is this weird DEBUG constant?
// Well its a technique that allows adept C
// programmers to enable or disable
// certain print statements throughout their
// code.

// Instead of manually deleting these statements
// to "stop debugging", you can simply
// change the value of DEBUG once at the top
// to immeadiately activate / deactivate all
// the relevant strings that trace your code.

/* creates storage and initializes length/size
returns 1 on success, 0 for failure*/
int al_init(arraylist_t *L, unsigned capacity)
{
    
    L->data = (elem_t *)malloc(capacity * sizeof(elem_t));
    // Allocates a chunk of memory assuming that the user
    // passed capacity indicates the number of elements
    // the arraylist can hold.

    // We cast the result of malloc (using (elem_t *) )
    // because malloc ALWAYS RETURNS A VOID POINTER.
    // A void pointer is an untyped pointer.
    // While in later versions of C, casting is not required
    // explicitly, it is still safer to cast it
    // to the type you desire to make full use of it.

    // This array space (or rather its first element)
    // will be linked with L->data. L->data when used,
    // will allow us to iterate over the arraylist
    // starting from the first element.

    // ANY DATA IS SIMPLY AN ARRAY OF BYTES, so we can use
    // array notation (e.g. L->data[1]) to traverse
    // the heap stored arraylist.


    if (L->data == NULL)
        return 0;

    // If malloc failed, then L->data will be null.
    // If we fail to create the space, then we return 0.

    L->length = 0;
    L->size = capacity;

    // In my opinion this was confusing but remember that in
    // this case, size is the max number of elements the array
    // can hold. The length is the current number of elements
    // the array holds.

    return 1;

    // If we got to this point, then hooray! Return 1 for success!
}

/* creates and pointer to an arraylist,
creates storage for it and initializes capacity,
and returns the pointer to the created arraylist
back to the user.*/
arraylist_t *al_create(unsigned capacity)
{

    arraylist_t *L = malloc(sizeof(arraylist_t));
    
    // Note that the only major difference between this
    // function and the al_init() function is that instead 
    // of letting the user create the pointer
    // externally and then pass it in, WE CREATE AND
    // INITIALIZE THE POINTER FROM WITHIN THE FUNCTION.
    
    if (L == NULL)
        return NULL;
    
    // If L, the pointer, is NULL,
    // then malloc() failed and we return NULL
    // to indicate failure.


    if (al_init(L, capacity))
    {
        return L;
    }

    // Otherwise if we did sucessfully create the pointer,
    // then we call al_init to handle initializing the pointer
    // as per usual. We return L if al_init is successful,
    // at which point we leave this functtion.

    // NOTE THAT THIS IS THE CLASSIC CASE OF A HELPER METHOD / FUNCTION.
    // al_init() does the majority of the work,
    // but this al_create() method simply exists
    // to alleviate the user from the pain of writing
    // the line to create a pointer.

    free(L);

    // If we have not left the function
    // after the previous if statement,
    // then al_init() must have failed.
    // In that case we first free() any
    // space we allocated (this prevents a memory leak)
    // and then exit by returning NULL to indicate failure.

    return NULL;
}

/*simply frees the data associated with the arraylist pointer*/
void al_destroy(arraylist_t *L)
{
    free(L->data);
    // This should be self-explanatory.
    // Calling free DOES NOT RESET DATA,
    // it unmarks the locations we were using.
    // Future C programs and executions 
    // can then safely write over our data when
    // they need space.
    
    // If we forget to free, then we are
    // basically "losing" memory
    // in the sense that we keep marking
    // new portions of memory as
    // in use even though in reality they are
    // not.
    // Do this enough times and your computer
    // will complain.
}

/*returns the length of the arraylist*/
unsigned al_length(arraylist_t *L)
{
    return L->length;

    // Self-explanatory.
}

/* adds specified element to end of list
, returns 1 on success, 0 on failure
(assumes list has been initialized). */
int al_push(arraylist_t *L, elem_t elem)
{
    // check whether array is full
    if (L->size == L->length)
    {
        // increase capacity
        int new_size = L->size * 2;
        elem_t *new_data = realloc(L->data, new_size * sizeof(elem_t));
        if (new_data == NULL)
            return 0;
        if (DEBUG)
            printf("Increased size to %d from %d\n", new_size, L->size);
        L->size = new_size;
        L->data = new_data;
    }
    L->data[L->length] = elem;
    L->length++;
    return 1;
}

/* pops the element at the end of the arraylist,
returns 1 on success or 0 if the arraylist was empty
prior to calling al_pop.*/
int al_pop(arraylist_t *L, elem_t *dest)
{
    if (L->length == 0)
        return 0;
    L->length--;
    if (dest)
        *dest = L->data[L->length];
    if (DEBUG)
        printf("Removed %d; new length %d\n", L->data[L->length], L->length);
    return 1;
}