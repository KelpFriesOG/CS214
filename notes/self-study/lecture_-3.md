# Why the hell is it C all over again?

This "lecture" is just to get you familiar with C and why and how you should use it.

Systems programming is (according to Wikipedia):

is the activity of programming computer system software. The primary distinguishing characteristic of systems programming when compared to application programming is that application programming aims to produce software which provides services to the user directly (e.g. word processor), whereas systems programming aims to produce software and software platforms which provide services to other software, are performance constrained, or both (e.g. operating systems, computational science applications, game engines, industrial automation, and software as a service applications).

Lets digest this babble in plain english:

- Systems programming is the process of making programs that help more complex programs (applications) function. These programs could be drivers (that allow your mouse, keyboard, GPU, e.t.c. to interface )

- To play a game (an app) you typically have to first make sure your mouse works and your downloaded game engine(s), i.e. DirectX, Vulkan, OpenGL, are compatible with the game.

**Nowadays both mouse drivers (the things that allow your mouse to move) and game engines, are built on C or similar performant languages!**

**Systems programming won't work well with Python, Java, or other languages but is does work well with C, lets see why! (C what I did there?)**

---

## Should I skip this shit?

If you have taken any Computer Architecture course you should have already seen C.

- *If you are confident that you understand the basic syntax of C and can write up a simple prototype, method, and struct, then please feel free to skip this whole file!*

- **If any of this sounds unfamiliar then I suggest skimming through at the least.**

---

## Why do we use C? (Short answer: memory management)

We use C, which by our standards is an archaic langauge, because C interfaces very closely with the actual hardware of the compiler.

**The fastest written code (with some exceptions), tends to be written in either some assembly language or C.** These languages trade syntactical complexity and programmer typing power for pure computational speed.

- In general, if the language require you to type more, it probably means the language is faster.

Why?: **As programming became more mainstream, more abstract languages such as Java and Python were created.** They were abstract in the sense that they required less typing and were more intuitive for the programmer to code. They also did more "work" with less lines of code.

- The downside is that **these languages are interpreted, which is to say that they go through many layers of translation before being executed on the machine** (**which makes them typically slower at execution**).

**The upsides include:**

1) More intuitive for programmers to code leading to faster development times.

2) More customizability and introduction of nice to have features which can assist in speeding up execution time to be on par with compiled code.

3) **Programmers don't have to manage memory manually on the computer!**

**Read the example below to understand why the last one is usually good for application programming cases but not for systems programming!**

---

Example:

The best example of an advantage of modern languages is the garbage collector.

- Java was the first notable language to have a fully fledged garbage collector, managed by the program the runs Java code: the Java Virtual Machine.

- The JVM's garbage collector allocates and frees up memory based how and when the user declares variables (in the scope of the code). The machine intelligently removes data / objects from memory if the code destroys any and all pointers to that data / object in memory.

- **Languages such as C have no such smart memory management feature**

**This is actually good in the context of systems programming because it allows us, the users, to be arbiters of memory. Our code can explicitly interface with system memory (RAM or storage).**

---

C also allows to interface with other very basic processes in our computer. This includes keypresses, **file management, signal processing** (i.e. keyboard drivers, modifying file properties, choosing your wifi network, maintaining your bluetooth connections can and is all be done with short, fast, and reuseable C code).

- **File management and signal processing are core to this course!**

---

## Quick Q & A

Some questions you may have:

1) *Is C object oriented?: Yes and No. **Probably No.***

- C was never designed to object oriented but rather it is procedure oriented.

According to wikipedia and most people:

"(OOP) is a programming paradigm using "objects" – data structures consisting of data fields and methods together with their interactions"

- **C has structs which are akin to objects in the sense that they have data fields. But otherwise, structs cannot contain methods. So yeah, we can't say structs are truely objects in the correct sense.**

Object oriented languages encourage standard practices such as: abstraction, encapsulation, modularity, polymorphism and inheritance!

- **C does not natively like to do any of this! We can try our damndest to get it there but it is hard as a mothafucka.**

For these reasons among a few more, C should not be classified as object oriented. **However, just because a language is or isn't object oriented does not make it "good" or "bad".**

2) *Is C necessary to be a good programmer: Yes and No. **Probably No**.*

Lets be honest here, you probably wanted to be a programmer to make the next big thing. You want to make some useable app that garners plenty of attention, recognition, fame, and most importantly money!

- System drivers, kernels, and game engines are not as flashy as Amazon, Facebook, or other apps / games. **But they are just as if not more quintessential then the apps themselves!**

- At the end of the day you may (in my opinion you *should*) decide that C is not for you.

- Choosing to criticize C is one thing, but if anything, let this course stick in your head to remind you that **thousands of system programmers work to make your mouse move as smoothly as it does and to let your games and apps behave without crashing.**

- Systems programming is thankless and bland work, but its good, effective and necessary work.

**And if you don't like C but like the concept and field of Systems programming, you're in luck!**

- Alternative, more easy to code languages, that are provably almost as if not as fast as C are becoming more popular! Look into Rust, Carbon, and C++ if this field intrigues you!

- Even modern language such as Python and Java, use C as their defacto language, because ultimately your high level code gets pushed down through the pipeline to end up as C code! 

- Developers of Python and Java are still working tirelessly to optimize these languages by utilizing C code where possible and emulating the similar speeds!

3) *Is C difficult to learn: Yes, unless you're hyper talented.*

- No elaboration needed, just try out coding it...

---

## Now we CODE.

---

Lets look at this sample code:

```
#include <stdio.h>
#include <string.h>

int main()
{
    printf("Hello World");
}

```

Now your spacing may vary but the basic syntax rules are:

1) Each declaration / action line must end with a semicolon.
2) Reuseable blocks of code (methods) must follow the following format

- *[static] returntype name( param1, param2, e.t.c ){ body }*

3) Lines that begin with # are **preprocessor** statements. These are statements that are read by the *linker* before the code itself is *assembled* and runs. (**If these words are gibberish just know that if it begins with # its special sauce**).

For those coming from Java, keep in mind the big difference:

- **C has no built in access modifiers! In other words, no private, protected, public, e.t.c.**

- **With the exception of static, which you can put before the return type of a method (this makes the method only accessible from within the file), or before the name of a variable (which makes the variable recall its value between invocations of the method).**

---

Here is a more complicated example with methods:

```

#include <stdio.h>

void foo()
{
    int a = 10;
    static int sa = 10;

    a += 5;
    sa += 5;

    printf("a = %d, sa = %d\n", a, sa);
}


int main()
{
    int i;

    for (i = 0; i < 10; ++i)
        foo();
}

```

Shamelessly copied from: [Eli Bendersky on Stack Overflow](https://stackoverflow.com/questions/572547/what-does-static-mean-in-c)

This code illustrates how we can write a method and proceed to use that method in the main method.

- In this case we use the underlying "foo" method 10 times via a for loop.
- **If you run this code is should show that while while $a$ simply increments from 10 to 15 in every run of the code, $sa$ actually keeps changing after and with each run!**

**This static behavior is useful when "a function needs to keep some state between invocations, and you don't want to use global variables" (same source as code).**

**Static methods and variables are also made to become file specific, meaning that other files cannot reference or access them. This is good practice, when needed, and is called encapsulation!**

---

What would happen if we put the chunk of main method code before the code it references? If this was Java, then nothing, but since its C...

```

#include <stdio.h>

int main()
{
    int i;

    for (i = 0; i < 10; ++i)
        foo();
}

void foo()
{
    int a = 10;
    static int sa = 10;

    a += 5;
    sa += 5;

    printf("a = %d, sa = %d\n", a, sa);
}

```

If your IDE and compiler are dumb and doesn't fix this mistake for you, you should get an error!

If your compiler is smart it may still give you a warning like:

*basics2.c:8:9: warning: implicit declaration of function 'foo' [-Wimplicit-function-declaration]*

*basics2.c:8:9: note: previous implicit declaration of 'foo' was here*

- C is a linearly read language, meaning that if the stuff that comes first depends on the stuff that it yet to be read, then the program throws you a big HUH?

- By the time we get to the for loop in the main method, we have yet to know what the hell foo() is, and what it do. **So when we call foo() nonetheless (in the for loop) the compiler freezes, tries determining if it has seen foo() yet, says NOPE, and crashes.**

How do we fix this behavior?

1) We could always write the methods in order of dependency. If method A depend on method B, then method A should be written first, then method B. 
2) We could use prototypes, that let the compiler know *how and where* to find the function in the code if it runs into a situation where it has not found it yet!

Approach 1 becomes very complicated as we introduce more methods that become more interrelated and dependent on each other.

**So instead the good practice is function prototypes (approach 2)!**

This is the fixed code:

```
#include <stdio.h>

void foo();

int main()
{
    int i;

    for (i = 0; i < 10; ++i)
        foo();
}

void foo()
{
    int a = 10;
    static int sa = 10;

    a += 5;
    sa += 5;

    printf("a = %d, sa = %d\n", a, sa);
}

```

Some quick notes on prototypes.

- The return type of the prototype must be the same as the type of the actual function you want the prototype to be associated with.
- The number, types, and order of parameters must be same in the prototype compared to the target function.

If you want to save your hands the effort:

1) You do not have to name the parameters in your prototype

**Ex: int add(int x, int y) is as equally effective as int add(int, int)**

2) If you decide to name your prototype's parameters, the actual function's parameters DO NOT need to have the same name.

**Ex: int add(int x, int y) effectively references the target function int add(int alpha, int beta)**

If ya don't believe me, then try out the following example code:



```

#include <stdio.h>

int add(int, int);

void foo();

int sub(int a, int b);

int main()
{
    int i;

    for (i = 0; i < 10; ++i)
        foo();
}

int add(int a, int b){
    return a + b;
}

int sub(int alpha, int beta){
    return alpha - beta;
}

void foo()
{
    int a = 10;
    static int sa = 10;

    add(a, 5);
    sub(sa, 5);

    printf("a = %d, sa = %d\n", a, sa);
}

```

You should run this program only to find out that once again, the static typing allows the value of $sa$ to change between invocations, whereas the value of $a$ does actually change due to the single add() call that occurs in every run of foo() which always sets $sa = 10 + 5$.

---

