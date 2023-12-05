#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define IS_INTERACTIVE (argc == 1)
#define IS_BATCH (argc == 2)
#define MAX_TOKENS 1024
#define DEBUG 0

#pragma region Function Prototypes and Globals

char **process_line(char *line);
char **find_glob(char *path, char *pattern);
int builtin_cd(char **args);
int builtin_pwd(char **args);
int builtin_which(char **args);
void builtin_exit(char **args);
int is_builtin(char **line);
int is_valid_path(char *path);
int exec_command(char **args, int argc);
char **glob_eval(char **args);

char *builtin_commands[] = {"cd", "pwd", "which", "exit"};

#pragma endregion

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

int exec_command(char **args, int argc)
{

#pragma region Shared Memory Stuff

    key_t key = ftok("shared_memory_key", 1234); // Generate a unique key
    int id = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (id == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment to the current process
    int *status = (int *)shmat(id, NULL, 0);
    if (status == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Initialize status
    *status = 0;

#pragma endregion

    // Evaluate globbing (wildcards)
    args = glob_eval(args);

#pragma region Special Cases

    // If the entire command array is 1 argument
    if (argc == 1)
    {
        // Execute the command (if it is builtin) (only works for pwd and exit)

        if ((*status = is_builtin(args)) == 0)
        {
            printf("check builtin");
            // builtin_exit(args);
            return *status;
        }
    }

    // If the entire command array begins with cd then
    // execute the cd command
    if (strcmp(args[0], "cd") == 0)
    {
        // Check if there is any symbol following the cd command and see if its a pipe
        if (argc > 2)
        {
            *status = 1;
            return *status;
        }

        builtin_cd(args);
        return *status;
    }

#pragma endregion

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

#pragma region Input Redirection

        // If input redirection symbol was found
        if (input_redirect_index != -1)
        {
            printf("input redirect index: %d\n", input_redirect_index);
            // Ensure that the filename after the symbol is valid
            if (input_redirect_index + 1 < i && args[input_redirect_index + 1] != NULL)
            {
                // Try to open the input file
                int fd = open(args[input_redirect_index + 1], O_RDONLY);
                if (fd == -1)
                {
                    *status = 1;
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
                *status = 1;
                fprintf(stderr, "Error: Input file not specified after '<'\n");
                exit(EXIT_FAILURE);
            }
        }

#pragma endregion

#pragma region Output Redirection

        // If output redirection symbol was found
        if (output_redirect_index != -1)
        {
            printf("output redirect index: %d\n", output_redirect_index);
            // Ensure that the filename after the symbol is valid
            if (output_redirect_index + 1 < i && args[output_redirect_index + 1] != NULL)
            {
                // Try to open the output file
                int fd = open(args[output_redirect_index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd == -1)
                {
                    *status = 1;
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                // Redirect STDOUT to output file
                dup2(fd, STDOUT_FILENO);

                // Close the file descriptor
                close(fd);

                // Remove redirection symbols and filenames from the args array
                args[output_redirect_index] = NULL;
                args[output_redirect_index + 1] = NULL;
            }
            else
            {
                *status = 1;
                fprintf(stderr, "Error: Output file not specified after '>'\n");
                exit(EXIT_FAILURE);
            }
        }

#pragma endregion

        // If the command is a builtin command, execute it and set status
        *status = is_builtin(args);

        if (*status == 1)
        {
            // reset status
            *status = 0;
        }

        // If the command can't execute successfully,
        if (execvp(args[0], args) < 0)
        {
            *status = 1;
            printf("failed to execute command: %s", args[0]);
            exit(EXIT_FAILURE);
        }
    }

    // If in the parent process
    else if (pid > 0)
    {
        // Wait for the child process to finish
        wait(NULL);
        return *status;
    }
    else
    {
        // Error
        perror("fork");
        *status = 1;
    }
}

char **glob_eval(char **args)
{
    char *cwd = getcwd(NULL, 0);
    char **new_args = NULL;
    int new_arg_count = 0;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (strchr(args[i], '*') != NULL)
        {
            // Expand the glob pattern
            char **glob_results = find_glob(cwd, args[i]);
            if (glob_results != NULL)
            {
                // Add each result to new_args
                for (int j = 0; glob_results[j] != NULL; j++)
                {
                    new_args = realloc(new_args, sizeof(char *) * (new_arg_count + 1));
                    if (new_args == NULL)
                    {
                        perror("realloc");
                        exit(EXIT_FAILURE);
                    }
                    new_args[new_arg_count++] = glob_results[j];
                }
                free(glob_results); // Free the array, not the strings
            }
        }
        else
        {
            // Copy the non-glob argument
            new_args = realloc(new_args, sizeof(char *) * (new_arg_count + 1));
            if (new_args == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            new_args[new_arg_count++] = strdup(args[i]);
        }
    }

    // Finalize new_args
    new_args = realloc(new_args, sizeof(char *) * (new_arg_count + 1));
    new_args[new_arg_count] = NULL;

    // Free the original args array, but not the strings
    free(args);

    return new_args;
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

// Function to check if a filename matches the pattern (assuming simple suffix matching)
int matches_pattern(const char *filename, const char *pattern)
{
    const char *asterisk = strchr(pattern, '*');
    if (!asterisk)
    {
        return strcmp(filename, pattern) == 0;
    }
    else
    {
        const char *suffix = asterisk + 1;
        size_t suffix_len = strlen(suffix);
        size_t filename_len = strlen(filename);
        return filename_len >= suffix_len && strcmp(filename + filename_len - suffix_len, suffix) == 0;
    }
}

/* Function to find matches to a glob pattern (which contains a SINGLE wildcard)*/
char **find_glob(char *path, char *pattern)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char **matches = NULL;
    size_t count = 0;
    size_t size = 10; // Initial size
    matches = malloc(size * sizeof(char *));
    if (!matches)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (matches_pattern(entry->d_name, pattern))
        {
            if (count >= size)
            {
                size *= 2;
                matches = realloc(matches, size * sizeof(char *));
                if (!matches)
                {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            matches[count++] = strdup(entry->d_name);
        }
    }
    matches[count] = NULL; // Null-terminate the array

    closedir(dir);
    return matches;
}

/* Built in command: cd*/
int builtin_cd(char **args)
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
            return EXIT_FAILURE;
        }
        // Print current working directory
        printf("%s\n", getcwd(NULL, 0));
    }

    return EXIT_SUCCESS;
}

/* Built in command: pwd*/
int builtin_pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("mysh");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* Built in command: which*/
int builtin_which(char **args)
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
        return EXIT_FAILURE;
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    return EXIT_SUCCESS;
}

/* Built in command: exit*/
void builtin_exit(char **args)
{
    exit(0);
}

/* Function to check if the provided command is valid
, if it is valid, execute the associated command and return 0, else return 1*/
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
            int status = 0;
            // Execute the command via a switch case based on the
            // current value of i.
            switch (i)
            {
            case 0:
                status = builtin_cd(line);
                break;
            case 1:
                status = builtin_pwd(line);
                break;
            case 2:
                status = builtin_which(line);
                break;
            case 3:
                builtin_exit(line);
                break;
            }

            return status;
        }
    }

    // Else return 1
    return 1;
}

/* Function to execute a batch file */
void execute_batch_file(FILE *fp)
{
    char *lineptr = NULL;
    size_t n = 0;
    int status = 0;

    while (getline(&lineptr, &n, fp) != -1)
    {
        if (lineptr[strlen(lineptr) - 1] == '\n')
        {
            lineptr[strlen(lineptr) - 1] = '\0';
        }

        char **tokens = process_line(lineptr);

        // Count up the number of tokens
        int num_tokens = 0;
        while (tokens[num_tokens] != NULL)
        {
            num_tokens++;
        }

        if (tokens != NULL)
        {
            if (!is_builtin(tokens))
            {
                status = exec_command(tokens, num_tokens);
            }
        }

        free(tokens);
    }
    free(lineptr);
}

// This function should handle the pipe logic
int execute_pipe(char **tokens, int argc)
{
    // Expand globs for the entire command line before splitting
    tokens = glob_eval(tokens);

    int status = 0;

    // Check if there is any symbol following the cd command and see if its a pipe
    if (argc > 2 && strcmp(tokens[0], "cd") == 0)
    {
        // Print da tokens
        // printf("tokens: %s %s %s\n", tokens[0], tokens[1], tokens[2]);

        if (strcmp(tokens[2], "|") == 0)
        {

            // Copy the tokens into a new malloced array
            char **new_tokens = malloc(sizeof(char *) * argc);
            for (int i = 3; i < argc; i++)
            {
                new_tokens[i - 3] = tokens[i];
            }

            // Free the original tokens
            free(tokens);

            // Execute the (pipeless) command assuming there
            // can be at most one pipe in any command.
            status = exec_command(new_tokens, argc - 3);

            printf("argc :%d", argc);

            return status;
        }
        else
        {
            status = 1;
            return status;
        }
    }

    // Find the pipe symbol and split the command
    int i;
    for (i = 0; tokens[i] != NULL; i++)
    {
        if (strcmp(tokens[i], "|") == 0)
        {
            tokens[i] = NULL; // Null-terminate the left command
            break;
        }
    }

    char **left_args = tokens;
    char **right_args = &tokens[i + 1];

    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        status = 1;
        return status;
    }

    pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork");
        status = 1;
        return status;
    }

    if (pid1 == 0)
    {
        // Child 1: Will execute the left side of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(left_args[0], left_args);
        perror("execvp");
        status = 1;
        return status;
    }

    pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    {
        // Child 2: Will execute the right side of the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        execvp(right_args[0], right_args);
        perror("execvp");
        status = 1;
        return status;
    }

    // Parent: Closing pipe and waiting for children
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

int main(int argc, char *argv[])
{

    // Initialize status and prompt variables
    char *prompt = "mysh> ";
    int status = -1;

    /* Check if the program is run in batch mode */
    if (IS_BATCH)
    {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            exit(1);
        }

        execute_batch_file(fp);
        fclose(fp);
        exit(0);
    }

    /* If the program is set to run in interactive mode */
    if (IS_INTERACTIVE)
    {

        char *lineptr = NULL;
        size_t n = 0;

        // Continue until user types exit
        do
        {
            // Print prompt
            printf("%s", prompt);

            // Get user input
            getline(&lineptr, &n, stdin);

            // Remove trailing newline
            if (lineptr[strlen(lineptr) - 1] == '\n')
            {
                lineptr[strlen(lineptr) - 1] = '\0';
            }

            // Break line into tokens
            char **tokens = process_line(lineptr);

            // Count the number of tokens
            int token_count = 0;
            while (tokens[token_count] != NULL)
            {
                token_count++;
            }

            // Check for a pipe symbol
            int has_pipe = 0;
            for (int i = 0; tokens && tokens[i] != NULL; i++)
            {
                if (strcmp(tokens[i], "|") == 0)
                {
                    has_pipe = 1;
                    break;
                }
            }

            // Execute commands based on whether a pipe is present
            if (has_pipe)
            {
                // Execute the pipe command
                status = execute_pipe(tokens, token_count);
            }
            else
            {

                // Handle non-pipe commands
                if (tokens != NULL)
                {
                    printf("status : %d\n", status);
                    // If first token is "then" or "else" and status is 0 or not 0 respectively.
                    if ((strcmp(tokens[0], "then") == 0 && status == 0) ||
                        (strcmp(tokens[0], "else") == 0 && status != 0))
                    {
                        printf("pre-tokens\n");

                        int count = sizeof(tokens) / sizeof(4);

                        // printf("%d", count);

                        // Copy over tokens into new arguments starting after the "then" or "else"
                        /*
                        char **new_tokens = (char **)malloc((token_count - 1) * sizeof(char *));
                        for (int i = 1; i < token_count; i++)
                        {
                            new_tokens[i - 1] = tokens[i];
                        }
                        */

                        printf("after tok");

                        // Execute the command
                        status = exec_command(tokens, token_count - 1);
                        continue;
                    }
                    else if (strcmp(tokens[0], "then") != 0 && (strcmp(tokens[0], "else") != 0))
                    {

                        status = exec_command(tokens, token_count);
                    }
                    else
                    {
                        continue;
                    }
                }

                printf("\n outside not null\n");
            }

        } while (1);

        free(lineptr);
        return 0;
    }
}