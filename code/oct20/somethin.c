#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 32

void accept_line(char *line)
{
    printf("Got a line %s\n", line);
    free(line);
}

int main(int argc, char **argv)
{

    char buf[BUFSIZE];
    int pos, bytes, fd, start, line_length = 0;
    char *line = NULL;

    if (argc > 1)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0)
        {
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    while ((bytes = read(fd, buf, BUFSIZE)) > 0)
    {
        // Iterate through until I find a newline
        // character
        start = 0;
        for (pos = 0; pos < bytes; pos++)
        {
            if (buf[pos] == '\n')
            {
                line_length = pos - start;
                line = malloc(line_length + 1);
                memcpy(line, buf + start, line_length);
                line[pos] = '\0';
                accept_line(line);
            }
        }
    }

    close(fd);

    // Some potential errors here
    // If your buffer picks up some character
    // upto the new line.

    // The next line that we create will include
    // the characters of the previous line,
    // Therefore we need to keep track of where
    // the line starts as well.

    // What if a line is longer than my buffer length?
    

    return EXIT_SUCCESS;
}