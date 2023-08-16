# C Pointers and Arrays

---

## Pointers and shit

Let's not make this more complicated than it needs to be:

- When you pass a value through a function in C, the
  function cannot modify the value that has been passed.

What does this mean?:

Look at the following sample code:

```

void foo(int);

int main(){

    int a = 10;
    foo(a);

    printf("%d", a);

}

void foo(int x){

    x = x + 5;

}

```

What will the value of x that gets printed?: 10!

- The x that we modify in the function is actually a *copy* of x. In other words, the parameter x, is a copy of the value that gets passed!

- This behavior, called pass by copy or pass by value, is common amongst primitive types in most modern languages!

- However, **we classify modern languages as pass by reference**, because if we pass an ***object*** as opposed to a primitive in a OOP language, **the value that  gets passed into the parameter is a reference or the actual physical address of the value in memory. This allows methods to modify an object in a method and this change actually affects the object for the rest of the code!**

Question now is, *if we want to modify variables as if they were objects in C, can we do that?*

**Yes, C supports referencing and a whole lot more. Languages like Java and Python don't like it when you try toying with memory but C welcomes you!**

Modified previous example:

```

#include <stdio.h>

void foo2(int*);

int main(){

    int a = 10;
    foo2(&a);

    printf("%d", a);

}

void foo2(int* y){
    
    *y = *y + 5;

}

```

This code works as intended because we pass and expect the address of $a$ to come into foo2(). We can use a * to then modify the value the address points to directly!

To understand this code we have to understand the language of pointers and references.

- The dereference operator (*) allows us to access the underlying value based on an address.

- It has another use. If you see it on the left hand of an assignment (ex. int* x = y) then it acts as a type. The "int *" lets us know that x is an integer pointer (pointers are simply variables that contain addresses).

- The "&" operator is used to get the address of a variable. In other words it tells you the location of the variable in memory. This is useful for declaring and assigning a value to a pointer.

Lets bring the two operators in a situation together:

```

#include <stdio.h>

int main(){

    int y = 15;
    int * x = &y;

    int * z = x;

    printf("%d", *z);

}

```

**Lets breakdown the code:**

1) First line creates y and gives it the value of 15.

2) The second line puts the memory address of y into x, which works because x is declared as an integer pointer. 

- If x was byte pointer would this program still run? If so, would it run and give us expected results? Try it out!


1) The third line copies the address value in x into z, in other words after this line x and z are both pointers which hold to the same address and thus point to the same value (that value being 15).

2) The last line uses "*" as a dereferencing operator to get the value which the pointer points to. We can print the dereferenced value as an integer.

- If we said *x instead of *z, would we still print the same value? Yes, after all x and z point to the same memory address and both expect and print out the entire integer (they know how to read the memory because we gave them the int type, so they know to pick up 4 bytes of memory after the initial address to grab one whole int).

---