# Not Object, but "Memory" oriented.

## Structs

As discussed before, there are no true "objects" in C that combine the functionality 
of methods and also have characteristics.

However if our goal is to simply create a structure that has some
characteristics ex. (A person *has* brown hair, blue eyes, and *is* a particular height),
then in C we can use structs.

**Structs are similar to objects, except these are only defined by their traits,
and cannot *do* any actions (i.e. no methods).**

- A struct is simply a new named type that is a consists of multiply other types.
- **The minimum memory required for a single instance of a struct is the
at least the sum of the memory required for each of the types it consists of.**

Structs are generally declared like so:

```

typedef struct obj {
    type1 trait1;
    type2 trait2;
    type3 trait3;
    ...
} OBJ;

```

For example here's a hypothetical struct for a dog:

```

typedef struct dog
{
    char name[31];
    char owner[31];
    float weight;
    char breed[31];

} DOG;

```
**The minimum memory required for a single DOG is 31 + 31 + (4*8) + 31 = 125 bytes**


A couple of things to note:

1) We only need to declare a struct once, preferably at the top of the file.
2) After declaring it, we can initialize an instance of a struct in the following manner:

**OBJ name = {t1, t2, t3, e.t.c}**

**Where t1, t2, and the rest are all values you want to assign to trait1, trait2, trait3 e.t.c.**

3) If we want access information from a struct instance, we can treat the name
of the struct as a sort of pointer! We use "." which is the member access operator.

Suppose we have a DOG object named duke and it has a trait which is a char[] called name.

To print the name of duke we can say:

```
printf("%s", duke.name)
```

However we cannot change an attribute using this operator.

In other words, this DOES NOT work:

```
duke.weight = 25.6;
```

4) We can modify a struct if we first make a pointer to it, and then
dereference that pointer to access the underlying values.

We know that a pointer to duke would look like the following:

```
DOG * duke_ptr = &duke
```

After this we can dereference the pointer and then use the member access
operator to modify the trait we want.

This would look like:

```
DOG * duke_ptr = &duke
*duke_ptr.weight = 25.6
```

Hopefully you can see that we can condense these two lines into a one liner:

```
*(&duke).weight = 25.6
```

**NEVER EVER RESORT TO DOING WHAT I JUST DID**. 

Use C's built in arrow operator on a pointer to the struct!

**The best code**:

```
duke_ptr->weight = 25.6
```

**So as best practice, if a function needs to modify a struct, we pass
a pointer to the struct as opposed to the struct itself!**


Here is an example that shows how a struct can be declared and used from within
other functions:

```
#include <stdio.h>

typedef struct dog
{
    char name[31];
    char owner[31];
    float weight;
    char breed[31];
} DOG;

void setDog(DOG *, char *, char *, float, char *);
void printDog(DOG);

int main()
{

    DOG dog1;
    DOG dog2;

    setDog(&dog1, "Summer", "Kelpo", 12.5, "Shiba");
    setDog(&dog2, "Winter", "Abhi", 10.1, "Boxer");

    printDog(dog1);
    printf("---\n");
    printDog(dog2);

}


void setDog(DOG *dogptr, char *name, char *owner, float weight, char *breed)
{
    strcpy(dogptr->name, name);
    strcpy(dogptr->owner, owner);
    dogptr->weight = weight;
    strcpy(dogptr->breed, breed);
}

void printDog(DOG dog)
{
    printf("Name: %s\n", dog.name);
    printf("Owner: %s\n", dog.owner);
    printf("Weight: %f\n", dog.weight);
    printf("Breed: %s\n", dog.breed);
}

```

**Note that if we simply want to print some traits we can pass in the DOG struct,
but if we want to modify some values we prefer that the pointer to the DOG be passed.**

Now an important question: **Could this be a valid replacement for the setDog method?**:

```
void setDog(DOG dog, char *name, char *owner, float weight, char *breed)
{
    DOG * dogptr = &dog;

    strcpy(dogptr->name, name);
    strcpy(dogptr->owner, owner);
    dogptr->weight = weight;
    strcpy(dogptr->breed, breed);
}
```

**Your answer should be NO, not at all!**

- You can try this out for yourself, I encourage it.

The reason this function won't work harps back to the core principle that
C is copy by value. **In other words, when you make a dogptr in the function,
that pointer ain't pointing to the original dog variable that exists outside
the scope of the function, instead it is referring to the copied value that
exists in the parameter dog which is in the scope of the function.**

- The parameter *dog* is a full clone of the global variable *dog*, hence they are
NOT the same object!

**If you pass a pointer, your local pointer is a clone of the pointer in the calling
function, but the values in the pointer you passed (the copied value) can access
the original object / array outside of the context of the function! That is why
we pass pointers, for arrays, and for structs.**

Also consider the fact that passing an entire struct requires a lot of memory overhead compared to passing a pointer to the same struct.

---



---

