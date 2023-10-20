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

- **The dereference operator (*) allows us to access the underlying value based on an address.**

- It has another use. **If you see it (the '\*') on the left hand of an assignment (ex. int\* x = y) then it acts as a type. The "int \*" lets us know that x is an integer pointer (pointers are simply variables that contain addresses and have an associated type).**

- **The "&" operator is used to get the address of a variable. In other words it tells you the location of the variable in memory. This is useful for declaring and assigning a value to a pointer.**

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

## Arrays...

The most important definition of an array for the purposes of system programming is:

**An array is a fixed, reserved portion of memory which is explicitly allocated by the user.**

- Memory is represented as a 1D array, if we wanna reserve memory, we gotta slice up that array!

Here is a basic example of how to declare and initialize arrays:

```

#include <stdio.h>

void printnums(int[], int);
void printchars(char*, int);

int main(){

    int nums[10] = {0, 2, 4, 6, 8};
    int bad_nums[] = {1, 3, 5, 7, 9};
    char letters[26];

    for(int i = 0; i < 26; i++){
        letters[i] = i + 65;
    }

    printf("array of nums: \n");
    printnums(nums, 10);
    printf("\narray of chars: \n");
    printchars(letters, 26);

}

void printnums(int* nums, int length){

    for(int i = 0; i < length; i++){
        printf("%d, ", nums[i]);
    }

}

void printchars(char* chars, int length){

    for(int i = 0; i < length; i++){
        printf("%c, ", *(chars + i));
    }

}

```

Let's break this down:

1) There are two ways to declare and initialize the array in the same line. **You can choose to give the array a size or omit the size entirely.** **If you choose to omit the size, the array size is based on the number of elements you chose to declare. If you include the size, and the declared array is under that size limit, then the rest of the array is filled with default values.**

In other words

```

int main(){

    int chars[] = {'a', 'b'}
    // This array's size is 2, meaning there is no space reserved after the 'b'
    int chars[10] = {'a', 'b'}
    // This array's size is 10, meaning that after the 'b' there is a series of 8 default values (for chars this is '\0') to fill up the rest of the space.

}

```

2) You can also choose to declare an array, without initializing it, as long as you ensure that it has some set capacity.

3) We can access and change values in an array pretty straightforwardly as seen in the for loop.

4) If we want to able to access and change an array in a function we MUST pass a pointer to the array. The name of an array acts as a pointer to the first element. **The prototypes may have the parameter as type[] or type\* . In both cases we are passing a pointer to the array!**

5) In printnums() we use traditional array subscript notation to print out the values. However, look at printchars() and we can actually treat the pointer as a pointer and dereference the pointer by taking advantage of pointer arithmetic!

What do I mean by pointer arithmetic?:

- A pointer is a memory address value but also has a type associated with it.

- What happens when we increment a pointer?

- Depending on the type of the pointer, +1 can increment the memory address the pointer holds by a varying amount of bytes. 

- **For an int pointer x, x + 1, is the memory address 4 bytes ahead of x. For a char pointer y, y + 1, is the memory address 1 byte ahead of y, e.t.c. The increment is intuitively based on the size of the datatype.**

- **x + 1, for any pointer x always gets us the address of the x+1th element in the array**!

So with this in mind, **printchars() first increments the pointer, keeping in mind that the value of the pointer, chars, points to the first element.**The value of i starts as 0, so when we dereference (char + 0), we get the value of the first element. **As i increments, we keep accessing, dereferencing, and printing the char+ith value until we hit the last value.**

---

The previous examples should demonstrate that arrays and pointers as concepts are very closely linked. 

- **An array is accessed by using pointers to each of its members, pointers themselves contain memory addresses that refer to locations in memory which in itself is the biggest array.**

---