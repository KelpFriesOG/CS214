# Memory Allocation and C

In C we use the **malloc()** function to reserve and allocate memory ahead of time.


*Why do we use malloc?*: 
- We can use memory effectively and sparingly while still avoiding errors that may be caused by insufficient memory.

The malloc function is in the stdlib header file and allocates runtime memory (the stack) as opposed to the heap.

Traditional declaration initializes values onto the stack

```
int i;
```

When I give the variable a value then values may get put in the heap if needed.

```
int i = 2013;
```

If we utilize malloc to a similar effect we would code:

```
int* j = malloc(sizeof(int));
```

In the line above there are two chunks of data being stored. The pointer gets stored on the stack, (the value of j shows you a stack address), and the memory it points to is some reserved piece in the heap.

Ex: ![Stack to Heap](../../images/StackToHeap.PNG)

If we want to free up that memory (basically empty it out), we call the free() function on any pointer that points to the allocated chunk of memory.

```
int* j = malloc(sizeof(int));
int* k = j;
*k = 100;
free(k);
```

When we call free on k, which is the same value as j, the effect is the same as it would be if I called free on j. Since both pointers point to the same location, calling free on either one works as intended.

Keep in mind that the pointers can be of any type, just ensure that the allocated space is the same as the size of the data type.

If we want to allocate space for a string that consists of 10 characters, how would we do it?:

```
char* newString = malloc(10*sizeof(char));
```

**We need to multiply the allocated space by 10, because the sizeof() function returns the # of bytes for one char (i.e. 1), we have to multiply this value by the number of chars we intend to store.**

Remember that **malloc() returns a pointer, but more specifically a VOID pointer** (or NULL if there is no space available).

You may be told to cast the return type to the desired pointer type, but with newer versions of C this should not pose a problem.

Here's how you would wanna be typesafe (if needed):

```
char* newString = (char*) malloc(10*sizeof(char));
```

---

The information below is sourced from:
https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=msvc-170

**When malloc(n) is called, *at least* n bytes are reserved, but more might be required for alignment and maintainence!**

- The malloc() function is actually used by the system to allocate storage for _environ, envp, and argv variables.

---



