#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int a[] = {1, 2, 4, 17, 256, 0, 1<<30};
    int i;

    int fd = open("output.txt", O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if (fd < 0) {
	perror("output.txt");
	exit(EXIT_FAILURE);
    }

    write(fd, a, 7 * sizeof(int));
    close(fd);

    return EXIT_SUCCESS;
}
