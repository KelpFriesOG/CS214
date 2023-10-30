#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "lines.h"

#ifndef DEBUG
#define DEBUG 1
#endif

LINES *lines_fdinit(int fd)
{
    LINES *l = malloc(sizeof(LINES));
    l->fd = fd;
    l->bufpos = 0;
    l->buflen = 0;

    return l;
}

LINES *lopen(char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1) return NULL;

    return lines_fdinit(fd);
}

void lclose(LINES *l)
{
    close(l->fd);
    free(l);
}

char *getline(LINES *l)
{
    int start, len;
    int searching = 1;
    int linelength = 0;
    char *line = NULL;
    if (l->buflen < 0) return NULL;

    while (1) {
	start = l->bufpos;
	while (searching && l->bufpos < l->buflen) {
	    searching = l->buf[l->bufpos] != '\n';
	    l->bufpos++;
	}

	if (DEBUG) printf("start=%d, pos=%d, searching=%d\n", start, l->bufpos, searching);

	// we are one past a newline or one past the end of the buffer
	// add any text we have passed to the line we are constructing
	len = l->bufpos - start;
	if (len > 0) {
	    line = realloc(line, linelength + len + 1);
	    memcpy(line + linelength, l->buf + start, len);
	    linelength += len;
	    line[linelength] = '\0';
	    if (DEBUG) printf("collected line length %d\n", linelength);
	}

	if (!searching) {
	    return line;
	}

	l->buflen = read(l->fd, l->buf, BUFSIZE);
	if (DEBUG) printf("refilling buffer, got %d bytes\n", l->buflen);
	if (l->buflen < 1) { 
	    l->buflen = -1;
	    return line;
	}
	l->bufpos = 0;
    }
}
