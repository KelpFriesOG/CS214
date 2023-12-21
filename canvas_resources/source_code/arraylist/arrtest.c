#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arraylist.h"

int real_main(int argc, char **argv)
{
    arraylist_t list;
    al_init(&list, 8);

    for (int i = 0; i < 20; i++) {
	al_push(&list, i);
    }

    int n;
    while (al_pop(&list, &n)) {
	printf("Popped %d\n", n);
    }

    al_destroy(&list);

    return EXIT_SUCCESS;
}



int main(int argc, char **argv)
{
    // arraylist_t *list = malloc(sizeof(arraylist_t));
    // al_init(list, 8);

    arraylist_t *list = al_create(8);

    for (int i = 0; i < 20; i++) {
	al_push(list, i);
    }

    int n;
    while (al_pop(list, &n)) {
	printf("Popped %d\n", n);
    }

    al_destroy(list);
    free(list);

    return EXIT_SUCCESS;
}
