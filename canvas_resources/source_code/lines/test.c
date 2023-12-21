#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lines.h"

int main(int argc, char **argv)
{
    char *fname = argc > 1 ? argv[1] : "input.txt";
    LINES *ls = lopen(fname);

    char *l;
    while ((l = getline(ls))) {
	printf(">>%s<<\n", l);
	free(l);
    }

    lclose(ls);

    return EXIT_SUCCESS;
}
