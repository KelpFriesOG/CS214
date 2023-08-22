# Macros

Think of macros as shortcuts that help you write programs more efficiently and effectively.

- Macros are similar to functions and variables in the sense that they are reuseable pieces of code that can store formulas or constant information.

Scenarios in which you would want to use a macro:

1) If one value keeps being used over and over again without being changed (i.e. a constant value such as PI), then make it a macro!

2) A one line function or formula that is repeatedly used can be made into a macro.

3) You could make a macro out of other macros.

These 3 scenarios and use cases correspond with the 3 major types of macros.

- **Object Like Macros**: Some value or code can be substituted with a shorter and more meaningfully named macro.

- **Function Like Macros**: The macro could accept parameters and act as a function.

- **Chain Macros**: You can use macros in other macros (often useful in mathematical formulas with multiple constants).

---

## Are Macros Used?

Yes, actually C has predefined macros that are available to use at any time in
program execution. These macros cannot be changed but are handy for many
things such as debugging, logging, and general testing.

Here is a table of some macros:

| Macro | Description |
| ----- | ----------- |
| \_\_LINE__ | Returns the line number on which the current line being executed is on.|
| \_\_FILE__ | Returns the name of the C file that is currently running. |
| \_\_DATE__ | Returns the current date. (format: MMM DD YYYY) |
| \_\_TIME__ | Returns the current time. (format: HH:MM) |
| \_\_STDC__ | Return 1 if the code so has been compiled successfully. |

---

## User Define Macros

Macros are meant to be ways for programmers to optimize their coding experience.

- **Macros are preprocessor statements, meaning that when you define them you have to start of with "#define".**

The macros are processed before the program executes, **and before the program runs the macros in your code are replaced by the values given in the #define statements**. (Hence the term *pre*processor)

Here is an example of all three types of macro usage in play:

```
#define PI 3.14
#define Volume(r) (4/3)*PI*(r*r*r)
```

Here is an example of illustrating the usage of predefined macros:

```
#include <stdio.h>

void main()
{

    // Example of predefined macros
    printf("This is line no.: %d\n", __LINE__);
    printf("Name of this file: %s\n", __FILE__);
    printf("Current Date: %s\n", __DATE__);
    printf("Current Time: %s\n", __TIME__);
    printf("Compilation success: %d\n", __STDC__); // 1
}

```
Example above was shameless copied from: https://www.scaler.com/topics/c/macros-in-c/


---