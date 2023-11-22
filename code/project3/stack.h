// Define the maximum size of the stack
#define MAX_SIZE 500

// Define a structure for the stack
typedef struct
{
    char *items[MAX_SIZE];
    int top;
} Stack;

// Initialize the stack
void init(Stack *s);

// Push an item onto the stack
void push(Stack *s, char *item);

// Pop an item off the stack
char *pop(Stack *s);

// Check if the stack is empty
int is_empty(Stack *s);