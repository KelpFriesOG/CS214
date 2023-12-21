typedef int elem_t;

typedef struct {
    elem_t *data;
    unsigned length;  // number of elements in list
    unsigned size;    // number of elements in array
} arraylist_t;

int al_init(arraylist_t *, unsigned);
arraylist_t *al_create(unsigned);
void al_destroy(arraylist_t *);

unsigned al_length(arraylist_t *);
int al_push(arraylist_t *, elem_t);
int al_pop(arraylist_t *, elem_t *);
