typedef int elem_t;
// This is basically creating a synonym for int
// We do this because in the future we plan to
// accomodate non int values, and we can quickly
// change the functionality of the code by simply
// modifying this typedef instead of every instant
// of "int".

typedef struct {

    elem_t *data;
    unsigned length;
    unsigned size;

    // We use unsigned to save space (we know length and 
    // size cannot be negative)
} arraylist_t;

int al_init(arraylist_t *, unsigned);
// This function simply takes a arraylist pointer
// and creates space in the heap for the arraylist
// (the max # of elements is specified by the unsigned value).

arraylist_t *al_create(unsigned);
// This function is just a more intuitive version of init.
// Instead of requiring the user to pass a created arraylist_t
// pointer, this method makes the pointer AND allocates
// space based on the passed unsigned value.
// It returns the pointer to the newly created arraylist_t

void al_destroy(arraylist_t *);
// This function destroys the arraylist by just calling
// free on the passed pointer.


unsigned al_len(arraylist_t *);
// This function returns the length of the arraylist
// based on the length parameter stored by definition.

int al_push(arraylist_t *, elem_t);
// This function returns 1 if the operation was a success,
// else it returns 0. In order to perform the operation
// it creates a new temp pointer via realloc.
// If this pointer is sucessfully created, then it
// properly reassigns the passed pointer and the
// actual traits of the struct.

int al_pop(arraylist_t *, elem_t *);
// This function returns 1 if the operation was a success,
// otherwise it returns 0. It first checks if the length is
// 0, if so, it returns a 0 because we cannot pop if no elements
// exist in the first place. Otherwise we decrement the length
// and pop the element at the Lth position, which is the value
// of the length (this works because we decremented the length
// BEFORE attempting to get the Lth element).

// You may be asking, how the user access the popped element?
// We assign the passed pointer to the popped element.
// If the user attempts to dereference the pointer after passing
// it to al_pop and getting a success, the value that the pointer
// references will be the popped value!


