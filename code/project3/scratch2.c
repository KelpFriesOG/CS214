#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define IS_INTERACTIVE (argc == 1)
#define IS_BATCH (argc == 2)
#define MAX_TOKENS 1024
#define DEBUG 1

// which ls > some_file
// pwd > tmp

// Function Prototypes
char **process_line(char *line);
char **find_glob(char *path, char *pattern);
void builtin_cd(char **args);
void builtin_pwd(char **args);
void builtin_which(char **args);
void builtin_exit(char **args);
int is_builtin(char **line);
int is_valid_path(char *path);
int exec_command(char **args);

// List of built-in commands
char *builtin_commands[] = {"cd", "pwd", "which", "exit"};

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

/* A function that executes a command based on a tokenized list */
int exec_command(char **args)
{

    // Create child process
    pid_t pid = fork();

    // If in the child process
    if (pid == 0)
    {

        // Locate input and output redirection
        int input_redirect_index = -1;
        int output_redirect_index = -1;

        int i = 0;
        while (args[i] != NULL)
        {
            if (strcmp(args[i], "<") == 0)
            {
                input_redirect_index = i;
            }
            else if (strcmp(args[i], ">") == 0)
            {
                output_redirect_index = i;
            }
            ++i;
        }

        // If input redirection symbol was found
        if (input_redirect_index != -1)
        {
            // Ensure that the filename after the symbol is valid
            if (input_redirect_index + 1 < i && args[input_redirect_index + 1] != NULL)
            {
                // Try to open the input file
                int fd = open(args[input_redirect_index + 1], O_RDONLY);
                if (fd == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                // Redirect STDIN to input file
                dup2(fd, STDIN_FILENO);

                close(fd);

                // Remove redirection symbols and filenames from the args array
                args[input_redirect_index] = NULL;
                args[input_redirect_index + 1] = NULL;
            }
            else
            {
                fprintf(stderr, "Error: Input file not specified after '<'\n");
                exit(EXIT_FAILURE);
            }
        }

        // If output redirection symbol was found
        if (output_redirect_index != -1)
        {
            // Ensure that the filename after the symbol is valid
            if (output_redirect_index + 1 < i && args[output_redirect_index + 1] != NULL)
            {
                // Try to open the output file
                int fd = open(args[output_redirect_index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                // Redirect STDOUT to output file
                dup2(fd, STDOUT_FILENO);

                close(fd);

                // Remove redirection symbols and filenames from the args array
                args[output_redirect_index] = NULL;
                args[output_redirect_index + 1] = NULL;
            }
            else
            {
                fprintf(stderr, "Error: Output file not specified after '>'\n");
                exit(EXIT_FAILURE);
            }
        }

        // Expand globs in the argument list
        i = 0;
        char *cwd = getcwd(NULL, 0);
        while (args[i] != NULL)
        {
            char **glob_results = find_glob(cwd, args[i]);

            // If in DEBUG MODE and find_glob returned NULL, print message
            if (DEBUG && glob_results == NULL)
            {
                printf("No matches for glob pattern: %s\n", args[i]);
            }

            // Replace the original glob pattern with the expanded filenames
            if (glob_results != NULL)
            {
                free(args[i]); // Free the original glob pattern
                args[i] = glob_results[0];
            }

            // Free the glob results
            int j = 0;
            while (glob_results != NULL && glob_results[j] != NULL)
            {
                free(glob_results[j]);
                ++j;
            }
            free(glob_results);

            ++i;
        }

        // Print the args if in debug mode
        if (DEBUG)
        {
            printf("Command: ");
            for (i = 0; args[i] != NULL; ++i)
            {
                printf("%s ", args[i]);
            }
            printf("\n");
        }

        // Execute the command
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);

        if (DEBUG)
        {
            printf("Command executed successfully\n");
        }
    }
    else
    {
        // Error
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

/* Function to redirect standard input/output of a program and execute it*/
int redirect_io(char *program, char **args, int in_or_out)
{
    // If the in_or_out is 0, then we redirect standard input using freopen
    if (in_or_out == 0)
    {
        freopen(program, "r", stdin);
    }

    // If the in_or_out is 1, then we redirect standard output using freopen
    if (in_or_out == 1)
    {
        freopen(program, "w", stdout);
    }

    // Execute the program in a child process
    exec_command(args);
}

/* Function to create a pipeline with two processes */
int create_pipeline(char **left_args, char **right_args)
{
    // Create a pipe
    int pipefd[2];

    // Sse pipe() to arrange that standard output for the first process will
    // be written to standard input for the second process.
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the first process
    pid_t pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // If pid1 is 0, then we are in the first child process
    if (pid1 == 0)
    {
        // We will only be using the write end of the pipe (close the read end)
        close(pipefd[0]);

        // Anything that goes in std output of the first process
        // will be redirected to the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // We can techinically close the write end of the pipe here
        close(pipefd[1]);

        // Execute the first command
        execvp(left_args[0], left_args);

        // If the command fails, print an error and exit
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Fork the second process
    pid_t pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // If pid2 is 0, then we are in the second child process
    if (pid2 == 0)
    {
        // We will only be using the read end of the pipe
        close(pipefd[1]);

        // Anything that goes in std input of the second process
        // will be redirected to the write end of the pipe.
        dup2(pipefd[0], STDIN_FILENO);

        // We can techinically close the read end of the pipe here.
        close(pipefd[0]);

        // Execute the second command
        execvp(right_args[0], right_args);

        // If the command fails, print an error and exit
        perror("execv");
        exit(EXIT_FAILURE);
    }

    // Close both ends of the pipe in the parent
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

// Function to check if a string is a valid path
int is_valid_path(char *path)
{
    if (access(path, F_OK) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
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

    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("mysh");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == 0)
    {

        // Redirect the stdoutput
        dup2(pipefd[1], STDOUT_FILENO);

        // Create the child process
        // execv("find", (char *const[]){"find", path, "-name", pattern, NULL}) == -1
        if (execl("/usr/bin/find", "find", path, NULL) == -1)
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
    }

    return "HELLO";
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
        int prev_status = 0;
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
                // // Check if the first token is a file glob
                // char **glob = find_glob(cwd, tokens[0]);

                // // If the glob is valid and non-empty,
                // // then print each file in the glob
                // if (glob != NULL && glob[0] != NULL)
                // {
                //     // Print each file in the glob
                //     for (int i = 0; glob[i] != NULL; i++)
                //     {
                //         printf("%s\n", glob[i]);
                //     }
                // }

                // // Otherwise, try to execute the command
                // else
                // {
                //     prev_status = exec_command(tokens);
                // }

                exec_command(tokens);
            }

        } while (1);

        return (0);
    }
}
