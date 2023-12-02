#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <glob.h>

int exec_command(char **args)
{
    /* Iterate through args and expand any glob patterns into new args */
    char **new_args = malloc(sizeof(char *) * 1024);
    int new_argc = 0;

    for (int i = 0; args[i] != NULL; i++)
    {

        // If the current arg contains a wildcard, expand it via glob
        if (strchr(args[i], '*') != NULL || strchr(args[i], '?') != NULL)
        {

            glob_t glob_result;
            int status = glob(args[i], 0, NULL, &glob_result);

            // If expansion was successful, copy over the results into new args
            if (status == 0)
            {

                for (int j = 0; j < glob_result.gl_pathc; j++)
                {

                    new_args[new_argc] = glob_result.gl_pathv[j];
                    new_argc++;
                }
            }
        }

        // Otherwise just copy over the argument
        else
        {
            new_args[new_argc] = args[i];
            new_argc++;
        }
    }

    // Add a null terminator to the end of the new args
    new_args[new_argc] = NULL;

    // Print the new args
    for (int i = 0; new_args[i] != NULL; i++)
    {
        printf("%s ", new_args[i]);
    }
}

char **process_line(char *line)
{
    // Tokenize the given line
    char **args = malloc(sizeof(char *) * 1024);

    int argc = 0;

    char *token = strtok(line, " ");

    while (token != NULL)
    {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " ");
    }

    args[argc] = NULL;

    return args;
}

int main(int argc, char *argv[])
{

    // If argcount = 1

    int exit_status = 0;

    if (argc == 1)
    {

        while (1)
        {
            printf("mysh> ");

            char *line = NULL;
            size_t len = 0;
            ssize_t read;

            read = getline(&line, &len, stdin);

            // Call process line to tokenize the line
            char **args = process_line(line);

            // launch the command
            exit_status = exec_command(args);
        }
    }

    else
    {

        // Run in BATCH mode
    }
}