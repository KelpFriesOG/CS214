## 9 September 2023

---

### recap of last lecture
---

C Types:
- numbers: int float double, enum
- arrays: the length of an array needs to be declared before runtime but that information is not stored at runtime. Arrays have dimension(s) and an array type.

- **If we pass an array to a function which may have a variable length, make sure to pass in the size as well!**

## Structures
---

There are two aggregate types in C: arrays an structs.

- Structs can contain data of varying types.
- A structure contains key value pairs of a where each pair contains a field name and its value.

How do we declare a structure?

    struct structure_name {

        field_type field_name;
        field_type2 field_name2;
    
    };

For example here is a point structure:

    struct point {

        double x;
        double y;

    };

Then we would need to initialize a point "object" separately:

    struct point my_location;

    \\ or

    struct point my_dest = { 11, -4 };

You can also technically initialize a struct by specifying only a few of the fields (as long as the unspecified fields have a default or null value to fall back on).

    struct point partial_point = { 25 }
    
    // The line above sets partial_point.x = 25
    // partial_point.y's value is not set.

**We can create structs that use other structs as fields! (Encapsulation)**

    struct rectangle {

        struct point topleft;
        double width, height;

    }

**The space occupied by a struct in memory is the sum of the space required by each of it's fields.**

In the case of the rectangle class we need 4 doubles: topleft.x, topleft.y, weidth, and height.

But there may also be some padding between the fields when stored. Why can't we skip the padding, C Standard says we cannot.

- **Fields are always stored in the order declared.**
- **No other data is included.**
- **There may be padding between fields.**

Struct variables are allocated like primitive types, meaning that **global structs behave like global variables and local structs behave like local variables**.

*Global struct behavior:*
- Exists during the entire runtime of the program.
- Cannot be created nor destroyed (while program is running).

*Local struct behavior:*
- Created when function begins
- Destroyed when exiting the function.

We typically declare structs outside of the main method, meaning the the structs that you may have written before are likely global structs.

Arrays can be put inside of structs:

    struct record {

        char name[200];
        other whatever;

    }

What if we declare something like this:

    struct point vertices[10];

This declares **an array of points and each point struct takes up 4 doubles of memory resulting in an overally array size of 40 doubles or 160 bytes.**

Similarly:

    struct shape {

        struct point vertices[10];
        enum color background;

    }

    struct shape list[10];

The code above creates a list of 10 shapes, each shape contains a list of 10 points which occupies 4*4*10 bytes = 160 bytes and an enum field. So a list of 10 shapes would need AT LEAST (4*4*10)*10 = 1600 bytes in memory.

---

## Unions

A typical structure has enough space for all of its fields. **A union is a special type of structure that can only store one of its specified fields at a time.**

    union int_or_float {

        int i;
        float f;

    }

    // The struct can store any of its element's variables but only one at a time.

This is how we would use the aforementioned union:

    union int_or_float foo;

    foo.i = 24;
    foo.f = 1.1;

No information regarding what type the union contains at runtime. **If we try to read the data in a union variable at runtime we need additional context to figure out the type of data it might be containing.**

Why do we have unions and when do we use them?:

- C was written with the context that the system may have limited hardware and space. **Maybe we have 2 variables that are never used at the same time, so using a union would minimize space for using them (without needing to initialize both variables).** Nowadays our compilers handle this for us and we have plenty of memory!

- We can use unions to grab information for a specific subtype. In other words suppose we expect to be storing information in one of three forms (we don't know which during compile time). We could use a union to properly create a variable that could handle either of the 3 protocols. Nowadays we could handle this as subclasses.

---

What if we write the following (don't do this in you code):

    foo.i = 20;
    printf("%f\n", foo.f);

Officially we have no idea what we'll see. Because the union contains an int but we want to print its data in "float" formatting. **So in theory the compiler would interpret the int's bytes as floats. But the modern compiler may mark this as a mistake.**

**If you like set theory, structs utilize AND to combine fields, unions represent OR where only one of the data types is chosen.**

Additional notes:
- you can nest unions, just like structs.

---

## Typedef keyword

The *typedef* keyword allows us to create a new name for an existing type.

Syntactically:

    typedef some_existing_type new_name;

For example:

    typedef struct point my_point;
    
    // The two lines below do the same thing
    struct point p1;
    my_point p1;

This can get confusing really quick:

    typedef double vector[3];
    // The dimension goes with the name, just as with variable declarations.

    // The lines below mean the same thing.
    double vec[3];
    vector vec;

    // What about this?
    vector matrix[3];
    // This creates a 3x3 matrix (2D array) of 3 vectors, where each vector contains 3 doubles.

We can combine the struct declaration into the typedef syntax to create an all-in-one solution to declare a struct and rename it in the same lines.

    typedef struct point_s{
        double x, y;
    } point_t;

You could also skip giving the struct name altogether and just give the anonymous struct a typedef name.

    typedef struct {
        double x, y;
    } point_t;

Do not assume that you can do type-checking with typedef. Typedef if literally just a new name for the type, it is purely for readability and convenience.

- Typedef has no impact on runtime performance.

Additional notes:
- Types created by the typedef don't exist until AFTER the typedef statement.

Would the following work?:

    typedef struct node_s {
        int data;
        node_t *next;
    } node_t;

No! Because node_t has not been defined when the compiler hits the line "node_t *next"!

Instead the working alternative would be:

    typedef struct node_s {
        int data;
        struct node_s *next;
    } node_t;

---

## Headers and CPP (C Preprocessor)

A typical C file contains two kinds of programs:
1) C Code
2) Preprocessor directives (the lines that start with #)

The C pre-processor modifies and tunes our source code before running our file.

The goal is to combine other libraries, prototypes or things that our main file needs to run before actually executing it.

### "#include" directive

You should have already seen:

    #include <stdio.h>

The include directive takes the specified file and pastes it's code into the current file prior to runtime. (THIS IS LITERALLY WHAT IT DOES CTRL-C and CTRL-V).

- Why do this?: These files are libraries which are often hundreds if not thousands of lines in length. No one wants to copy and paste these manually.

You may also see:

    #include "file"

This finds the specified file name in the same directory as the current source file.

These files that we retrieve, be it library files or your own custom files are called ***header files***.

- Header files never contains C code that creates things.

- These files only contain code for naming, type information, and other static information.

To access a function or variable, the compiler goes through your codes top down file to file. The compiler never goes backwards.

- If a function calls another then function that is being called should be defined before the calling function is defined.

- If A calls B, then B should be declared before A.

Problem: **Mutual Recursion**

Suppose you have two functions that call each other.

How do you put the one the calls the other one first (because they each call each other, but then they cannot both be "first").

**To fix this paradox we can use prototypes.**

Example:

    void foo(int); // This is the prototype

    void bar(int x){
        foo(x);
    };

    void foo(int x){
        bar(x);
    }

In order to use standard library functions within our source code we use (by doing #include) a header file from C's built in libraries.

- The **built-in header files are always simply declarations that point to the standard library which is already pre-compiled**.

- If you look at stdio.h you will not see any defining code, it will just be a bunch of names and prototypes.

- Functions are declared in the header file not defined.

- In theory you could copy the C library into your source code but your code would be enormous and take much longer to compile (this is assuming you would have enough space, which would be impossible a few decades ago). **Instead we use header files to pick and choose functions we need from a precompiled C library.**

- You can have your include statements in the header files, but if you include the same library in a header file and in the source file which uses the header file you may have some issues.

- By the time we get to actual compilation, all the preprocessor directives have been transformed into real C code!

### "#define" directive

This directs the preprocessor to rename portions of the code. It simply searches and replaces.

Syntactically:

    #define name replacement_value

Example:

    #define STRING_LENGTH 100

The define statement's name must be a single token made up of letters, digits, and _.

- The convention is to use allcaps for the naming.
- The define statement looks for AN EXACT match of the name. 

Example of using the previous define statement example:

    for(int i = 0; i < STRING_LENGTH; i++;){

    }

What most people forget is that the define statement can also be used to replace entire expressions instead of single variables!

Example:

    #define POSITIVE > 0

    if (p POSITIVE){
        blah...
    }





---

