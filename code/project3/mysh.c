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

/* Function that processes a line and returns a list of jobs to execute*/
JOB *process_line(char *line)
{
    // Tokenize the line into a list of arguments
    // If a character is <, >, or |, treat it as its own argument
    char *token = strtok(line, " \t\n");
    char *args[MAX_TOKENS];
    int i = 0;

    while (token != NULL && i < MAX_TOKENS)
    {
        // If the current character is <, >, or | add the single character
        // to the list of tokens
        if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 || strcmp(token, "|") == 0)
        {
            args[i] = token;
            i++;
            continue;
        }

        args[i] = token;
        token = strtok(NULL, " \t\n <  >  | ");
        i++;
    }

    args[i] = NULL;

    // print the tokens we found (upto the last NULL)
    if (DEBUG)
    {
        for (i = 0; i < MAX_TOKENS; i++)
        {
            if (args[i] == NULL)
            {
                printf("NULL\n");
                break;
            }
            printf("%s\n", args[i]);
        }
    }

    return (JOB *)args;
}

JOB *process_line2(char *line)
{

    // READ THROUGH LINE CHARACTER BY CHARACTER
    char **stringList = NULL;
    int length = 0;
    int start = 0;
    int end = 0;
    int i;

    // Iterate through the string character by character
    for (i = 0; i <= strlen(line); i++)
    {

        if (line[i] == ' ' || line[i] == '\0' || line[i] == '\n' || line[i] == '<' || line[i] == '>' || line[i] == '|')
        {
            // Calculate the length of the batch
            int batchLength = end - start;

            // Skip leading whitespaces
            if (batchLength > 0)
            {
                // Allocate memory for the batch of characters
                char *batch = (char *)malloc((batchLength + 1) * sizeof(char));

                // Copy the batch of characters
                strncpy(batch, line + start, batchLength);
                batch[batchLength] = '\0';

                // Reallocate memory for the string list
                stringList = (char **)realloc(stringList, (length + 1) * sizeof(char *));
                stringList[length] = batch;
                length++;
            }

            // If the character was a <, >, or |, make a new batch
            // containing only that character
            if (line[i] == '<' || line[i] == '>' || line[i] == '|')
            {
                stringList = (char **)realloc(stringList, (length + 1) * sizeof(char *));
                stringList[length] = (char *)malloc(sizeof(char));
                stringList[length][0] = line[i];
                stringList[length][1] = '\0';
                length++;

                // If the next character beyond the current one is also <, >, or |,
                // print an error and abort function
                if (line[i + 1] == '<' || line[i + 1] == '>' || line[i + 1] == '|')
                {
                    // Print the two invalid characters one after another
                    printf("mysh: syntax error \n");
                    return NULL;
                }
            }

            // Move the start pointer to the next character after the whitespace
            start = i + 1;
        }

        // Move the end pointer to the next character
        end = i + 1;
    }

    // IF DEBUG, Print the strings in the list
    if (DEBUG)
    {
        for (i = 0; i < length; i++)
        {
            if (stringList[i] == NULL)
            {
                printf("NULL\n");
                break;
            }

            printf("%d:%s\n", i + 1, stringList[i]);
        }
    }

    // Free the memory allocated for the string list and batches
    for (i = 0; i < length; i++)
    {
        free(stringList[i]);
    }

    free(stringList);

    return 0;
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

            // Break line into tokens where each job has its associated files.
            JOB *job = process_line2(lineptr);

            // Check if user wants to exit
            if (strcmp(lineptr, "exit") == 0)
            {
                break;
            }

            // Otherwise execute check if the command is builtin
            // If not try executing the command as a non-built in command.
            if (is_builtin(lineptr))
            {
                char *args[2];
                args[0] = lineptr;
                args[1] = NULL;
                if (strcmp(lineptr, "cd") == 0)
                {
                    builtin_cd(args);
                }
                else if (strcmp(lineptr, "pwd") == 0)
                {
                    builtin_pwd(args);
                }
                else if (strcmp(lineptr, "which") == 0)
                {
                    builtin_which(args);
                }
                else if (strcmp(lineptr, "exit") == 0)
                {
                    builtin_exit(args);
                }
            }
            else
            {
                // Execute command
                execute_command(lineptr);
            }

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