// C file which creates a parent process and a child process,
// and uses the pipe function to send data between them.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd[2];

    pipe(fd);

    if (fork() == 0)
    {
        close(fd[0]);
    }

    close(fd[1]);
    return 0;
}