#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd, i, bytes;
    char *fname = argc > 1 ? argv[1] : "output.txt";

    fd = open(fname, O_RDONLY);
    if (fd < 0) {
	perror(fname);
	exit(EXIT_FAILURE);
    }

    while ((bytes = read(fd, &i, sizeof(i))) > 0) {
	printf("Read %d\n", i);
    }
    if (bytes == -1) {
	perror(fname);
	exit(EXIT_FAILURE);
    }

    close(fd);

    return EXIT_SUCCESS;
}
