#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{

    int fd = open("small", O_RDONLY);
    pid_t child = fork();

    if (child == 0)
    {
        // We added this line below
        // pause();

        // We added this line below
        // Print out child process number
        printf("I am child %d\n", getpid());

        char buf[5];

        int bytes = read(fd, buf, 4);

        printf("child read %d bytes %s \n", bytes, buf);

        return EXIT_SUCCESS;
    }

    int wstatus;
    pid_t waited_child = wait(&wstatus);

    // Keep the child on the OS

    printf("Child %d has terminated\n", waited_child);

    if (WIFEXITED(wstatus))
    {
        printf("Child %d exited with status %d\n", waited_child, WEXITSTATUS(wstatus));
    }
    else if (WIFSIGNALED(wstatus))
    {
        printf("Child %d was terminated by signal %d\n", waited_child, WTERMSIG(wstatus));
    }

    char buf[5];
    int bytes = read(fd, buf, 4);
    printf("child read %d bytes %s \n", bytes, buf);
}