// Implemented functions of a stack
#include "stack.h"

// Initialize the stack
void init(Stack *s)
{
    s->top = -1;
}

// Push an item onto the stack
void push(Stack *s, char *item)
{
    s->top++;
    s->items[s->top] = item;
}

// Pop an item off the stack
char *pop(Stack *s)
{
    char *item = s->items[s->top];
    s->top--;
    return item;
}

// Check if the stack is empty
int is_empty(Stack *s)
{
    return s->top == -1;
}
