#include "mysh.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define IS_INTERACTIVE (argc == 1)
#define IS_BATCH (argc == 2)
#define MAX_TOKENS 1024
#define DEBUG 1

// List of built-in commands
char *builtin_commands[] = {"cd", "pwd", "which", "exit"};

/* Function to execute non-builtin commands via execvp*/
void execute_command(JOB *job)
{
    pid_t pid;
    int status;
    if ((pid = fork()) == 0)
    {
        if (execvp(job->command, job->arguments) == -1)
        {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("mysh");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return;
}

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

// function that simply tokenizes from the given input stream
char **tokenize(FILE *stream)
{
    int i = 0;
    char *lineptr;
    size_t n = 0;
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    if (tokens == NULL)
    {
        perror("mysh");
        exit(1);
    }

    // Use read() function to read a stream in a buffered manner
    while ((getline(&lineptr, &n, stream)) > 0)
    {
        tokens[i] = lineptr;
        i++;
    }

    // Print the tokens (DEBUG MODE)
    if (DEBUG)
    {
        for (i = 0; i < MAX_TOKENS; i++)
        {
            if (tokens[i] == NULL)
            {
                printf("NULL\n");
                break;
            }
            printf("%s\n", tokens[i]);
        }
    }

    return tokens;
}

// Main function
int main(int argc, char **argv)
{
    char *prompt = "mysh> ";

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

    /* If this program is set to run in interactive mode, enter a do-while loop*/
    if (IS_INTERACTIVE)
    {
        char *lineptr;
        size_t n = 0;
        char *cwd = getcwd(NULL, 0);

        // Continue until user types exit
        do
        {
            // Print prompt
            printf("%s", prompt);

            // Get user input
            getline(&lineptr, &n, stdin);

            // Check if user wants to exit
            if (strcmp(lineptr, "exit") == 0)
            {
                break;
            }

            // Tokenize user input
            char **tokens = tokenize(stdin);

        } while (1);
    }

    // Prompt to print
    // char *prompt = "mysh> ";

    // printf("%s", prompt);

    // char *lineptr;
    // size_t n = 0;

    // getline(&lineptr, &n, stdin);
    // printf("%s\n", lineptr);

    // free(lineptr);

    return (0);
}
