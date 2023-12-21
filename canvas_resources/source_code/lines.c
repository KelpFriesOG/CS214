#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define DEBUG 1

#define BUFSIZE 16

void accept_line(char *line)
{
    printf("Got a line: |%s|\n", line);
    free(line);
}

int main(int argc, char **argv)
{
    char buf[BUFSIZE];
    int pos, bytes, line_length = 0, fd, start;
    char *line = NULL;

    if (argc > 1) {
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
	    perror(argv[1]);
	    exit(EXIT_FAILURE);
	}
    } else {
	fd = STDIN_FILENO;
    }

    while ((bytes = read(fd, buf, BUFSIZE)) > 0) {
	if (DEBUG) printf("Read %d bytes\n", bytes);
	// search input for a newline character
	start = 0;
	for (pos = 0; pos < bytes; pos++) {
	    if (buf[pos] == '\n') {
		line_length = pos - start;
		line = malloc(line_length + 1);
		memcpy(line, buf + start, line_length);
		line[line_length] = '\0';
		accept_line(line);
		start = pos + 1;
	    }
	}
    }

    close(fd);

    return EXIT_SUCCESS;
}
