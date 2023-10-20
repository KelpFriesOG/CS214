#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{

    int value;
    struct Node *next;

} Node;

Node *makeLL(int value)
{

    // Creating a new node in the HEAP by
    // creating a pointer that will point
    // to a section of the heap that has
    // been reserved for a piece of data
    // which has the same size as the Node.

    // Note that since malloc returns a void
    // pointer we have to cast to a Node pointer.

    Node *head = (Node *)malloc(sizeof(Node));

    head->value = value;

    head->next = NULL;

    return head;
}

void addNode(Node *node, int value)
{

    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;

    while (node->next)
    {
        node = node->next;
    }

    node->next = newNode;
}

int printLL(Node *head)
{

    Node *tracer = head;
    int count = 0;

    while (tracer != NULL)
    {
        count++;
        printf("%d: %d, ", count, tracer->value);
        tracer = tracer->next;
    }

    return count;
}

void freeLL(Node *head)
{

    Node* next;

    while (head != NULL)
    {
       next = head;
       head = head->next;
       free(next);
    }
}

int main()
{

    printf("Is this thing working\n");

    Node* head = makeLL(10);

    for (int i = 0; i < 10; i++)
    {
        addNode(head, i);
    }

    printLL(head);

    freeLL(head);

}