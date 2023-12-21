#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define DEBUG 1

#define BUFSIZE 256

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
		int len = pos - start;
		line = realloc(line, line_length + len + 1);
		memcpy(line + line_length, buf + start, len);
		line_length = line_length + len;
		line[line_length] = '\0';

		accept_line(line);

		line = NULL;
		line_length = 0;
		start = pos + 1;
	    }
	}
	if (DEBUG) printf("End of buffer; start=%d pos=%d\n", start, pos);

	// check for a partial line
	if (start < bytes) {
	    int len = bytes - start;
	    line = realloc(line, line_length + len + 1);
	    memcpy(line + line_length, buf + start, len);
	    line_length += len;
	    line[line_length] = '\0';
	    if (DEBUG) printf("Partial line |%s|; %d bytes\n", line, line_length);
	}


    }

    close(fd);

    return EXIT_SUCCESS;
}
