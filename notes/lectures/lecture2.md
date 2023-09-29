## 15 September 2023

Recap:
- Preprocessor statements happen before compilation.
- Compiling with "-E" will run the preprocessor and print the transformed source
  code.

- We covered #include and #define.
- "#define" only replaces the strict name outside of the context of strings!
- This means that variables that contain the name of the token and other chars,
  the preprocessor does not change the name. Same thing with string literals.

---

- What if you want to replace a macro in a string tho?

You can use the macro to replace a string literal if you surround the string
literal (in the code) that you want to replace with quotes.

You can also define multiline macros by using the MULTILINE keyword.

- You can also tell "#define" to deal with arguments.

    #define SQUARE(X) X * X

    y = SQUARE(not_x);

    // Gets turned into...

    y = not_x * not_x;


However this could cause problems with improper code:

- There is no type checking involved.

Ex:

    #define SQUARE(X) X * X

    SQUARE("Hello");

    // transforms into...

    "hello" * "hello";

The macro also does not care about order of operations!

Ex:

    #define SQUARE(X) X * X

    1 / SQUARE(X);

    // This would transform it into

    1 / X * X

    // What we wanted was 1 / (X * X)


We could fix this, right...

    #define SQUARE(X) (X * X)

    y = SQUARE(a + b);

    // transforms into

    y = (a + b) * (a + b)

    // This is again not what we want.
    // We wanted (a + b)^2

The final and proper version is:

    #define SQUARE(X) ((X * X))


Another set of preprocessor directives set the macro
code based on a conditional execution.

These include: ifdef, else, endif, ifndef, if

These are typically used when writing library code that has
to deal with the possiblity of multiple operating systems.

- For us, this is useful for debugging the code if we choose to.

useful debugging trick:

    #idndef DEBUG
    #define DEBUG 0
    #endif

    if(DEBUG) { printf(); }

If I ran compiler with -DDEBUG, all the debugging code will be included
by default. (-D is a compiler option to define a macro at runtime).

- Doing -D on a macro add the macro defined as 1. You can also set the
  the macro to a specified expansion.

In the scope of this class we should use macros for constants so that
if our number needs to change we simply need to change it in one place.

---

## Memory and Pointers

All data is in memory.
- Unique items in memory are called objects.

Strictly speaking an object is just a contingious range of bytes used
to store a single item.

**Every variable has an associated object that stores its value.**

**Every function has an object that stores its code.**

**Some objects are not associated with a variable or name.**

**We can refer to objects based on their starting locations (memory address)**

The memory address refers to the byte in memory which is the first byte
of the object. **The address of the least significant byte is considered to** 
**be the address of the whole object.**

- Object can be nested: an array object contains smaller element objects.
- Structs contain the addresses of the objects of various types.

To use an object we need to know its location and type! The type information
is not kept in memory, it is understood at compilation (via the types we give).

If we want to find an object we need a type and address, this is the goal
of pointers.

**Pointers are typed objects that hold the address of some other object.**

The "&" operator allows us to get the address of any variable.

Ex:
    int x = 3
    printf("%h", &x);

&x is called a pointer, in this case a pointer to x.

**We can more formally define a pointer variable** and 
initialize it to the address of some variable.**

Ex:
    int x = 3;
    int* y = &x;

In this situation y is a pointer that will hold the same value to 
&x contains. (&x and y hold the same address: the address of x).

- The &x contains type information based on the type of x! In this case
  both &x and *y are both "int *" (AKA int pointers).

Suppose we get the pointer, what can we do with it?

Well we can pass it into functions, we can do some special arithmetic,
or we can dereference it!

1) The "*" **operator** is a unary operator that dereferences the value at proceeds it.
- This means that assigning a value to *y would be the same as assigning to x (going of
  the previous example).

Ex:

    int x = 3;
    int *y = &x;

    *y = 10

    printf("Value: %d", x)
    // This prints out 10, because we changed the value at x.



What if:

Ex:
    int* p, q;
    // p is a pointer to an int, but q is just an int!

    // This is why the convention is int *p.
    int *p, *q;

Why use pointers if we already have variable names?

1) Consider the *new* keyword in Java! When we create an object via
   the new keyword we have to assign it a name on the left hand side.
   The new keyword is making space in memory for this object.

2) We use pointers to name memory which is freed to be used at runtime 
   (after you hit play). There is a specify way to code that allows you
   to allocate memory at runtime (malloc()).

---

## Arrays

Array names are treated like pointers.

    char s[] = "some string";
    char d[STRLEN];

    char *p, *q;

    for (p = s, q = d; *p != '\0'; p++, q++){
        q* = *p
        // 2 BUGS
        // q does not get the terminator byte, we end when p holds the terminator byte.
        // We also could have a situation where STRLEN is not big enough to hold all of s.
    }

If p points to an object then p+1 points to the next object (the pointer knows how
many bytes to hop based on the type of the pointer).

---

## Malloc

The function malloc allocates space on the heap. The compiler does not know ahead of time
how much space we are allocating, this is decided at runtime.

- In order to allocate an object, you need to know how big it is.

To get the size of a type using the sizeof function: sizeof(<type>)

    sizeof(int);
    sizeof(float);
    sizeof(struct Node);

The malloc() function returns a "void *" which is an untyped pointer.
However when we assign the return of malloc it is autocasted into the right type.

    struct node *head = malloc(sizeof(struct node));


However we could intentionally fuck things up:

    double *foo = malloc(sizeof(int));
    // This is undefined behavior and the compiler
    // won't protect you (unless you have sanitizers
    // that address it).


Writing your own malloc() - Project 1





---
