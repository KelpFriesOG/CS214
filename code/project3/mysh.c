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

// which ls > some_file
// pwd > tmp

// Function Prototypes
void execute_command(JOB *job);
void *dispatch_jobs(char **tokens);
char **process_line(char *line);
char **find_glob(char *path, char *pattern);
void builtin_cd(char **args);
void builtin_pwd(char **args);
void builtin_which(char **args);
void builtin_exit(char **args);
int is_builtin(char **line);

// List of built-in commands
char *builtin_commands[] = {"cd", "pwd", "which", "exit"};

/* Function to execute non-builtin commands via execv*/
void execute_command(JOB *job)
{
    pid_t pid;
    int status;
    if ((pid = fork()) == 0)
    {
        if (execv(job->command, job->arguments) == -1)
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

/* Takes in a token list and returns a list of jobs
if each section of the list is a valid job*/
void *dispatch_jobs(char **tokens)
{

    // Setting up a pointer to iterate through the tokens.
    char *next_tkn = tokens + 1;
    char *current_tkn = tokens;
    char *prev_tkn = NULL;

    // Using a while loop to continue iterating until current token is NULL
    while (*current_tkn != NULL)
    {

        // If the token is contains a wildcard "*" then expand it to
        // all the files it refers to within the current directory
        if (strchr(*current_tkn, '*') != NULL)
        {
            // Find the glob pattern, and create a job for each associated file
            char **glob = find_glob(".", *current_tkn);
        }

        // TODO: Deal with redirection and piping tokens "<, >, |"

        // Otherwise simply check if the given token is a file that exists
        // within the current working directory
        else
        {
            if (access(*current_tkn, F_OK) == -1)
            {
                printf("mysh: %s: No such file or directory\n", *current_tkn);
            }
        }

        // If in DEBUG mode, print the token
        if (DEBUG)
        {
            printf("%s\n", *current_tkn);
        }

        // Iterating the tokens
        prev_tkn = current_tkn;
        current_tkn = next_tkn;
        next_tkn = next_tkn + 1;
    }

    return NULL;
}

/* BETTER Function that processes a line and returns a pointer to the tokens*/
char **process_line(char *line)
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

    // Insert a NULL at the end of the list
    stringList = (char **)realloc(stringList, (length + 1) * sizeof(char *));
    stringList[length] = NULL;
    length++;

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

    return stringList;
}

/* Function to find matches to a glob pattern (which contains a SINGLE wildcard)*/
char **find_glob(char *path, char *pattern)
{
    // Pattern matching (w/o using glob function)
    // Create child process that calls find with the given path
    // and glob pattern (using execv)

    // Then we pipe the result of the command to be returned from this function

    // Create a child process
    pid_t pid;
    int status;

    if ((pid = fork()) == 0)
    {
        // Create a pipe
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("mysh");
            exit(EXIT_FAILURE);
        }

        // Create the child process
        // execv("find", (char *const[]){"find", path, "-name", pattern, NULL}) == -1
        if (execl("/usr/bin/find", "find", path, "-name", pattern, NULL) == -1)
        {
            perror("COULD NOT FIND FILE: mysh");
            exit(EXIT_FAILURE);
        }

        // Close the write end of the pipe
        close(pipefd[1]);

        // Read the output from the child process
        char buffer[1024];
        char **output = NULL;
        int i = 0;

        while (read(pipefd[0], buffer, sizeof(buffer)) > 0)
        {
            // If DEBUG, print the output
            if (DEBUG)
            {
                printf("%s", buffer);
            }

            // Add the output to the list
            output = (char **)realloc(output, (i + 1) * sizeof(char *));
            output[i] = buffer;
            i++;
        }

        // Close the read end of the pipe
        close(pipefd[0]);

        // Return the outputted list of files
        return output;
    }
    else
    {
        // Wait for the child process to finish
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) != 0)
            {
                perror("mysh");
            }
        }

        return NULL;
    }
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
        // Print current working directory
        printf("%s\n", getcwd(NULL, 0));
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
    // Check the actual bin directory to ensure the command exists
    // use a child process to execute the which command
    pid_t pid;
    int status;
    if ((pid = fork()) == 0)
    {
        if (execvp("which", args) == -1)
        {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, &status, 0);
    }
    return;
}

/* Built in command: exit*/
void builtin_exit(char **args)
{
    exit(0);
}

/* Function to check if the provided command is valid
, if it is valid, execute the associated command and return 1, else return 0*/
int is_builtin(char **line)
{
    int i;

    // Extract first token as the comman
    char *command = line[0];

    // Iterating through built in commands
    for (i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]); i++)
    {
        // If we find a matching command return 1
        if (strcmp(command, builtin_commands[i]) == 0)
        {

            // Execute the command via a switch case based on the
            // current value of i.
            switch (i)
            {
            case 0:
                builtin_cd(line);
                break;
            case 1:
                builtin_pwd(line);
                break;
            case 2:
                builtin_which(line);
                break;
            case 3:
                builtin_exit(line);
                break;
            }

            return 1;
        }
    }

    // Else return 0
    return 0;
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

            // Remove trailing newline using simple logic
            if (lineptr[strlen(lineptr) - 1] == '\n')
            {
                lineptr[strlen(lineptr) - 1] = '\0';
            }

            // Break line into tokens

            char **tokens = process_line(lineptr);

            // If the command is a built in command,
            // execute it and move on.
            if (tokens != NULL && is_builtin(tokens))
            {
                continue;
            }
            else
            {
            }

        } while (1);
    }

    return (0);
}