September 6, 2023

---

## What is the C Programming Language?

-> Group programming class (groups of two).

The first C compiler and other older projects were feasibly written by a single person over the course of months.

- In the modern day, modern programs would take months if not years to code alone, hence most work is done under a group.

Write clear, commented, understandable code.

- **Your audience should be able to figure out your code without much guidance.**

Currently the plan is having a midterm and a final.

- We may switch to having frequent quizzes and one final.

- We will use iLab machines as reference hardware. These are located on the second floor of the Hill Annex.

- You can login remotely on a webpage or using SSH.

Need to make a separate account for the CS Lab using the same NetID and a different password (if you so choose).

The Command Center cluster is what is reccomended to use.

C calls POSIX API to execute its commands on the hardware level.

---

## Topics

1) C Programming
2) POSIX OS Calls (File I/O, Directory Handling)
3) Multitasking
4) Multithreading (PThreads)
5) Network Communication (Sockets)

---

## C Programming Language

Lets contrast it with Java to gain some familiarity:

Java has a JVM but C does not utilize a virtual machine.

- The JVM executes bytecode on internal hardware.

- The JVM is written in C / C++.

- IN a C Environment we write C source code and compile using a compiler such as GCC which takes the code and translates it into assembly (hardware language).

- We skip the step of having any sort of virtual machine and get a significant speed upgrade.

For C, the compiler varies depending on the hardware to ensure portability (depending on the assembly architecture).

What is the downside?:

- If we want to run C code made on one machine on another we may have to recompile the code.

We can compile the source code from ANY text editor.

The virtual machines already have some text editors installed like nano.

We can use the "cat" command in a UNIX environment to print a text file.

---

CC = gcc
CFLAGS = -g (helps with debugging), -Wall (Turns on most warnings)m -Weverything (Turns on all warnings, not reccomended)

With C you can make little mistakes
which can only be found if you turn
on warnings.

Ex. 2^n does not do what you think, it does
not raise 2 to a power (^ is the XOR operator).

-Werror (If there are any warnings the code will not compile, turn it on or off based on preference.)

-fsanitize=address (Enables address sanitizers which means it adds a bunch of code that ensures you are fucking around with memory)

If fsanitize is not enabled then C does NO MEMORY CHECKING.

- These are runtime checks, so they take up more memory and time (for us this is not a problem).

FIX THE PROBLEMS FOUND BY ADDRESS SANITIZER. 

-fsanitize=undefined (Enables undefined behavior sanitizer) Ex. If two signed integers sum up to an OVERFLOW error then having this enabled will catch that!

Again this slows down and bulks up your program but again it is necessary to prevent unpredicatble behavior

FIX THE PROBLEMS FOUND BY UNDEFINED SANITIZER.

We can combine multiple sanitizers like so:

-fsanitize=address,undefined

Sometimes these sanitizers may result in false positives or miss mistakes but this is rare. However nothing in class will be so advanced to cause such issues.

--

Makefiles allow us to specify options for the compiler and reuse these options without typing it all over again.

---

## More Differences Between C and Java

In C local variables are unintialized by default.

Why?: Speed!

- There may be situations where you may declare a variable and never initialize it.

Global variables are always initialized to 0 or it's equivalent in other data types.

C has many variations of int:

*signed* / *unsigned* int

*unsigned* by itself means unsigned int

*long* / *short* int

short -> at least 2 bytes.
int -> at least 2 at most 4 bytes.
long int -> at least 4 bytes.

char is a 1 byte character (not UNICODE, ASCII)

- Chars can be signed or unsigned (depending on the system). If we only care about ASCII then signage does not matter.

No bool:
    - We use an integer: 0 is false anything else is true.

---

