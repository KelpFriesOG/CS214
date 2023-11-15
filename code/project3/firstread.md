# Making your own Shell system

Your shell should be able to do the following:

- POSIX (unbuffered) stream IO
- cd, pwd
- create child processes and get their exit status
- Use dup2() and pipe()
- Read the contents of a given directory

---

## Batch mode vs interactive mode

**If mysh is called with an argument it opens a file,
and interprets its lines as a series of commands.**

**If mysh is called without an argument it enters
interactive mode. Here the user specifies commands
to be executed, and waits for the program to give
back the results.**

How do I exit the shell from interactive mode?
- Create a "exit" command.

How does it exit form batch mode?
- mysh terminates once it reaches the end of the input file.

---

## Usage

### Batch mode:

```

$ cat myscript.sh
echo hello
$ ./mysh myscript.sh
hello
$

```

### Interactive mode:

```

$ ./mysh
Welcome to my shell!
mysh> cd subdir
mysh> echo hello
hello
mysh> cd subsubdir
mysh> pwd
/current/path/subdir/subsubdir
mysh> cd directory_that_does_not_exist
cd: No such file or directory
mysh> cd ../..
mysh> exit
mysh: exiting
$

```

---

## Command Formatting

- One command per line, each command describes a job.

Each job may involve many child processes.

mysh needs to determine the following:

- How many child processes to spawn
- What each child process will do
- For each process, the path to the program to execute
- The arguments to pass to the program
- Which files to use for input and output

**Each command is made up of tokens, each token is a sequence
of non-whitespace characters.**

Exceptions:
- <, >, | are always treated as tokens of their own.

Example:
`foo bar<baz`
- The line above has 4 tokens: `foo`, `bar`, `<`, and `baz`.

**We build the list of arguments as we step through the tokens**

### Wildcards

**FOR SIMPLICITY WE WILL ASSUME THAT PATTERNS CONTAIN ONLY
ONE ASTERISK. THE WILDCARD ONLY APPLIES TO FILENAMES WITHIN
THE SAME DIRECTORY.**

### Redirection

- The < and > tokens are used for input and output redirection respectively.

**THE FIRST ARGUMENT FOLLOWING THE REDIRECTION TOKEN IS NOT
ADDED TO THE ARGUMENT LIST, instead IT IS KEPT AS THE
REDIRECTION FILE NAME. TOKENS AFTER THIS FILENAME ARE
TREATED AS NORMAL ARGUMENTS.**

### Pipelines

- The | token is used to create a pipeline. The file on the left of this token is piped into the file on the right of the token.

- Each program in the pipeline will have a separate argument list and file redirection targets.

- **We will use the pipe() command to put the standard output of the first program into the input of the second program.**

**FOR SIMPLICITY LIMIT MYSH TO PIPELINES INVOLVING ONLY TWO PROGRAMS**

### Conditionals

then ...

Only execute ... if the previous program exited successfully.

else ...

Execute ... if the previous program exited not normally.

---




---

## Q & A

1) What if the file contains the exit command and then a few other commands after? When does batch mode stop reading?

2) General structure of what we need to tackle?

Major components:

- Command line parser (includes wildcard expansion)

Program launcher
    - create child process
    - change stdin/stdout in child
    - gather exit status from child

**REMINDER regarding execv()**

Type signature:

```
execv(char *path, char **args);
```

**Last argument SHOULD ALWAYS BE NULL**

3) What function should I use for the built in cd and pwd functions?

- use the getcwd() function for pwd
- use the chdir() function for cd

4) Should I use execvp or execv?

- I (the professor) wants us to use execv.

5) Is there some sort of reccomended structure?

tokenizer (could just be the main method):
- repeatedly calls read to get input text
- breaks inputs into tokens.
- creates a job order describing the process to launch.
- job order includes argument list, input redirect, output redirect, and path to executable.

note: you will need one of these per process to launch.
- a pipeline will have at least two.

program launcher:
- create child process
- change stdin/stdout in child
- gather exit status from child


