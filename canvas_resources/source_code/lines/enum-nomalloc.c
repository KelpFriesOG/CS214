#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define DEBUG 1
#define BUFSIZE 16

int main(int argc, char **argv)
{
    int fd;
    if (argc > 1) {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
    } else {
        fd = STDIN_FILENO;
    }


    int pos = 0;
    int bufsize = BUFSIZE;
    char *buf = malloc(bufsize);
    int bytes;
    int start = 0;
    while ((bytes = read(fd, buf+pos, bufsize-pos)) > 0) {
        int buflen = pos + bytes;
        if (DEBUG) printf("Got %d bytes; %d/%d/%d/%d\n", bytes, start, pos, buflen, bufsize);
        // scan for end of line
        while (pos < buflen) {
            if (buf[pos] == '\n') {
                if (DEBUG) printf("Line; %d/%d/%d/%d\n", start, pos, buflen, bufsize);

                // for this example, we can just put the terminator directly in
                // the buffer; note that the newline gets dropped
                buf[pos] = '\0';
                printf("line: >>%s<<\n", buf+start);

                // set up for following line
                start = pos + 1;
            }
            pos++;
        }
        int leftovers = pos - start;
        if (DEBUG) printf("%d leftover; %d/%d/%d/%d\n", leftovers, start, pos, buflen, bufsize);

        if (leftovers == bufsize) {
            // token too large for the buffer
            bufsize *= 2;
            if (DEBUG) printf("Expand buffer to %d\n", bufsize);
            buf = realloc(buf, bufsize);
        } else if (leftovers > 0) {
            // partial token at end of buffer
            memmove(buf, buf + start, leftovers);
            start = 0;
            pos = leftovers;
        } else {
            // token ended exactly at end of buffer
            start = 0;
            pos = 0;
        }
    }
    if (bytes < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // FIXME: should check for leftovers here

    free(buf);
    close(fd);

    return EXIT_SUCCESS;
}

