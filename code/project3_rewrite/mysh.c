#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>

void process_line(char *line, char ***args, size_t *arg_count);
void exec_command(char **args);
void handle_builtin_commands(char **args);
void expand_globs(char ***args, size_t *arg_count);
void free_args(char **args);

int main()
{
    char *line = NULL;
    size_t line_size = 0;

    while (1)
    {
        printf("sh> ");
        if (getline(&line, &line_size, stdin) == -1)
        {
            break; // EOF or error
        }

        // Remove newline character from input
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        char **args = NULL;
        size_t arg_count = 0;

        process_line(line, &args, &arg_count);
        expand_globs(&args, &arg_count);
        handle_builtin_commands(args);
        exec_command(args);

        // Free allocated memory for arguments
        free_args(args);
    }

    // Free allocated memory for line
    free(line);

    return 0;
}

void process_line(char *line, char ***args, size_t *arg_count)
{
    char *token;
    *arg_count = 0;

    // Tokenize the input line
    token = strtok(line, " ");
    while (token != NULL)
    {
        *args = realloc(*args, (*arg_count + 1) * sizeof(char *));
        if (*args == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        (*args)[*arg_count] = strdup(token);
        if ((*args)[*arg_count] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        (*arg_count)++;
        token = strtok(NULL, " ");
    }

    *args = realloc(*args, (*arg_count + 1) * sizeof(char *));
    if (*args == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    (*args)[*arg_count] = NULL; // Null-terminate the argument list
}

void expand_globs(char ***args, size_t *arg_count)
{
    for (size_t i = 0; i < *arg_count; ++i)
    {
        if (strchr((*args)[i], '*') != NULL || strchr((*args)[i], '?') != NULL)
        {
            glob_t glob_result;
            if (glob((*args)[i], GLOB_NOCHECK, NULL, &glob_result) == 0)
            {
                // Free the original argument only if there are globs to expand
                free((*args)[i]);

                // Resize the argument list
                *args = realloc(*args, (*arg_count + glob_result.gl_pathc) * sizeof(char *));
                if (*args == NULL)
                {
                    perror("Memory allocation error");
                    exit(EXIT_FAILURE);
                }

                // Copy the expanded paths into the argument list
                for (size_t j = 0; j < glob_result.gl_pathc; ++j)
                {
                    (*args)[*arg_count + j] = strdup(glob_result.gl_pathv[j]);
                    if ((*args)[*arg_count + j] == NULL)
                    {
                        perror("Memory allocation error");
                        exit(EXIT_FAILURE);
                    }
                }

                *arg_count += glob_result.gl_pathc;

                // Free memory allocated by glob
                globfree(&glob_result);

                // Remove the original glob pattern argument
                for (size_t k = i; k < *arg_count - glob_result.gl_pathc; ++k)
                {
                    (*args)[k] = (*args)[k + glob_result.gl_pathc];
                }
                i--; // Recheck the current index
            }
        }
    }
}

void exec_command(char **args)
{
    pid_t pid;
    int status;

    if (args[0] == NULL)
    {
        return; // Empty command
    }

    if ((pid = fork()) < 0)
    {
        perror("Fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        execvp(args[0], args);
        perror("Exec error");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        waitpid(pid, &status, 0);
    }
}

void handle_builtin_commands(char **args)
{
    if (args[0] == NULL)
    {
        return; // No command
    }

    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "cd: missing argument\n");
        }
        else
        {
            if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
        }
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
        else
        {
            perror("getcwd");
        }
    }
    else if (strcmp(args[0], "which") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "which: missing argument\n");
        }
        else
        {
            char *path = getenv("PATH");
            char *token = strtok(path, ":");

            while (token != NULL)
            {
                char command_path[1024];
                snprintf(command_path, sizeof(command_path), "%s/%s", token, args[1]);
                if (access(command_path, X_OK) == 0)
                {
                    printf("%s\n", command_path);
                    return;
                }

                token = strtok(NULL, ":");
            }

            fprintf(stderr, "which: %s not found in PATH\n", args[1]);
        }
    }
}

void free_args(char **args)
{
    for (size_t i = 0; args[i] != NULL; ++i)
    {
        free(args[i]);
    }
    free(args);
}
