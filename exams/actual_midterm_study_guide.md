## Given topics

CS 214 / 2023-10-27
===================


Midterm Exam
------------

Friday, Nov 3, in class (this classroom, 2:00 PM - 3:20 PM)

you may bring one (single-sided) page of notes
they are unlikely to help you

C programming
    - general syntax
        - preprocessor, use of #include
    - standard library
    - helpful tools: gcc, make, man
        -> man (the manual) is divided into sections
            POSIX functions are in section 2
            C standard functions are in section 3
    - declarations and definitions
            extern int global_var;
    - preprocessing vs compiling vs linking
    - types
        - char, int (various), float, double
        - struct, union, enum
        - arrays
        - pointers
        - sizeof()
                int a[5], *p = a;
                sizeof(a) == 5 * sizeof(int)
                sizeof(p) == sizeof(int *)
                sizeof(*p) == sizeof(int)
    - macros
        - macro expansion
        - considerations when defining macros with arguments
        
                #define SQUARE(X)  ((X) * (X))
                SQUARE(x++)
                ((x++) * (x++))
                SQUARE(time_consuming_function())
                
        - checking macro expansion with gcc -E
        - use of \ to continue definitions

Memory 
    - understanding all data as collections of bytes
    - pointer vs "pointee"
    - pointer types and pointer arithmetic
    - object location vs object content
        int a;   a vs &a 
        int *p;  p vs *p vs &p
    - use of malloc(), realloc(), and free()
        int *p = malloc(...);
        int *q = p;
        p = realloc(p, ...)
        *q   /// potentially use-after-free

Posix File IO
    - open(), close(), read(), write()
    - use of buffers
    - opendir(), readdir(), closedir()

File system
    - inode block vs data block
    - directory files
    - "linking" files
        -> can a file have more than one name?
        -> can a directory be linked more than once?


Using directories
-----------------

a directory is a file containing directory entries
    each directory entry has (at least)
        a name
        an inode number
    it may also include
        file type
            regular file (contains data)
            directory
            soft link
            block device
            named pipes/FIFO
        
#include <dirent.h>

DIR *opendir(char *pathname);
    returns a pointer to an abstract struct
    or NULL on failure

struct dirent *readdir(DIR *directory_pointer);

    returns a pointer to a directory entry structure
    --> this may be overwritten the next time we call readdir()
    --> do not free()
    
    entries are not returned in any particular order
    
    
    struct dirent {
        ino_t  d_ino;   // the inode number for the entry
        char[] d_name;  // the name (NULL-terminated string)
        
    // not guaranteed to exist
        unsigned char d_type;  // the file type
    }


int closedir(DIR *directory_pointer);
    closes the directory
    returns 0 on success, -1 on failure (and sets errno)



.
|
+ input.txt
|
+ foo
  |
  + input.txt
  
---

## What to actually remember (a few important points)

- Remember how to use gcc and man linux commands

---

Syntax: gcc filename.c filename.h -o filename

Remember some basic flags for gcc:

-Wall: This will check not only for errors but also for all kinds warning like unused variables errors, it is good practice to use this flag while compiling the code. 

-Werror: This will check for all kinds of errors.

-E: This command will generate a preprocessed version of the source.c program. (helps check macro expansion)

-std=c11 :This command will use the c11 version of standards for compiling the source.c program, which allows to define variable under loop initializations also using newer standards version is preferred.

---

Syntax: man <section-number> <command-name>

or man <command-name>

man 3 fopen: This command will show the man page for the fopen function. Section #2 is the man page for POSIX functions, section #3 is the man page for C standard functions.

---

Recall basic makefile structure:

Ex:

CC      = gcc
CFLAGS  = -g -fsanitize=address,undefined -Werror
RM      = rm -f

default: all

all: Hello

Hello: memgrind.c
	$(CC) $(CFLAGS) -o main memgrind.c

memgrind.c : mymalloc.h mymalloc.c

clean:
	@echo "Cleaning up..."
	rm main

---

Preprocessing evaluates preprocessor commands into the C file
(aka. macros and includes) before compilation.

The gcc command we typically use compiles and links within
the same line. The process of compiling simply turns the
C code into assembly code. The process of linking turns
the assembly code into an executable (.o file).

---

Understand why macros NEED proper parentheses to work as intended.

KEY: **ORDER OF OPERATIONS**

Example:

#define RADTODEG(x) ((x) * 57.29578)

This defines a radians-to-degrees conversion which can be inserted in the code where required, i.e., RADTODEG(34). This is expanded in-place, so that repeated multiplication by the constant is not shown throughout the code. **The macro here is written as all uppercase to emphasize that it is a macro, not a compiled function.**

The second x is enclosed in its own pair of parentheses to avoid the possibility of incorrect order of operations when it is an expression instead of a single value. **For example, the expression RADTODEG(r + 1) expands correctly as ((r + 1) * 57.29578); without parentheses, (r + 1 * 57.29578) gives precedence to the multiplication.**

Similarly, the outer pair of parentheses maintain correct order of operation. **For example, 1 / RADTODEG(r) expands to 1 / ((r) * 57.29578); without parentheses, 1 / (r) * 57.29578 gives precedence to the division.**

Common pitfalls of macros:

Using a macro that calls a function that takes a long time to evaluate, twice or more with the same arguments.

EX: 
    #define SQUARE(X) ((X) * (X))
    SQUARE(time_consuming_function())
    evaluates to: ((time_consuming_function()) * (time_consuming_function()))

- This causes the compiler to need to execute the function twice.
  DO NOT assume that the function is only evaluated once, because
  macros are just tokenized replacements, they are not smart enough
  to understand that they can execute once and use the result many
  times for their own definition.

Using a macro with incremental evaluation. (UNDEFINED BEHAVIOR)

EX: 
    #define SQUARE(X) ((X) * (X))
    SQUARE(x++)
    evaluates to: ((x++) * (x++))

- Which incrementation happens first, when the does the multiplication happen? Honestly, the behavior is undefined. Suppose x = 2, then
would the result be (2\*2) 4 or (3\*3) 9 or (2\*3) 6 or (3\*4) 12?

---



