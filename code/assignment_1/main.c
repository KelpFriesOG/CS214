#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


void *malloc(size_t size) {
  
  // We use sbrk to either change the
  // heap size (with a positive argument)
  // Or it returns a pointer to the start of
  // new region on the heap.
  void *p = sbrk(0);
  
  void *request = sbrk(size);
  
  // If the request returns a pointer
  // equal to the value of the void pointer
  // -1 then sbrk could not allocate space
  // on the heap.
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  } 
  
  // Otherwise check if the 
  else {
    assert(p == request); // Not thread safe.
    return p;
  }

}