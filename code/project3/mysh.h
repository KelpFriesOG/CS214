// A job struct that contains a command, arguments, input redirect and output redirect,
// and a path to an executable
typedef struct job
{
    char *command;
    char **arguments;
    char *input;
    char *output;
    char *exec_path;
} JOB;