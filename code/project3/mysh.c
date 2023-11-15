#include "mysh.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define IS_INTERACTIVE (argc == 1)
#define IS_BATCH (argc == 2)

// List of built-in commands
char *builtin_commands[] = {"cd", "pwd", "which", "exit"};

/* Function to check if the provided command is valid */
int is_builtin(char *command)
{
    int i;

    // Iterating through built in commands
    for (i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]); i++)
    {
        // If we find a matching command return 1
        if (strcmp(command, builtin_commands[i]) == 0)
        {
            return 1;
        }
    }

    // Else return 0
    return 0;
}

/* Built in command: cd*/
void builtin_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "mysh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("mysh");
        }
    }
    return;
}

/* Built in command: pwd*/
void builtin_pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("mysh");
    }
    return;
}

/* Built in command: which*/
void builtin_which(char **args)
{
    int i;
    for (i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]); i++)
    {
        if (strcmp(args[1], builtin_commands[i]) == 0)
        {
            printf("%s\n", builtin_commands[i]);
            return;
        }
    }
    printf("mysh: %s: command not found\n", args[1]);
    return;
}

/* Built in command: exit*/
void builtin_exit(char **args)
{
    exit(0);
}

// Main function
int main(int argc, char **argv)
{

    /* Is this program is run in batch mode, try to open
     * the file specified in argv[1] */

    if (IS_BATCH)
    {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            exit(1);
        }
    }

    // Prompt to print
    char *prompt = "mysh> ";

    printf("%s", prompt);

    char *lineptr;
    size_t n = 0;

    printf("%s", prompt);
    getline(&lineptr, &n, stdin);
    printf("%s\n", lineptr);

    free(lineptr);

    return (0);
}