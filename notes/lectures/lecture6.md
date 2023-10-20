## Job Control and the shell

- A shell is  the general interface to the computer.

The program you run when you're not running a specific program.

You start jobs by entering commands into the shell:

(command name) (arguments)

Examples:

    cd some_dir

    cp some_file
    # A "bare name" program

If we use the bare name for a program then the shell looks in a set default place
for the file. The shell has a "search path" which includes the standard directories
to check for the existence of files.

"echo path" shows you the current search path.

A shell is simply a program written in another language (i.e UNIX is written in C).

- sh
- bash
- csh
- tcsh
- zsh
- fish

The stuff we mention typically works with all shells.

- You could write a shell in python for the heck of it.

The which command checks the working directory for to find what program will
be used when you run a specific command.

If you run:

    which ls

Then the result you should get is a location in usr/bin which includes
important runnable command-lets or cmdlets.

To run a program in the working directory you can just do:

    ./someprogram.o

The program will run if it is in fact runnable (executable).

If the program is in a subdirectory relative to the working directory then:

    p1/someprogram.o

**All paths are resolved relative to the current working directory.**

The shell does processing on all the commands.
- wildcard expansion. (wildcards are replaced by a space-separated list of matching files).

Example: 

    ls foo.*
    # actually may turn into ls foo.c foo.h foo.o

- escape characters (we can escape (ignore or account for) punctuation using '\')

Example:

    vim file\ name\ with\ spaces.txt
    # To see what this does
    echo file\ name\ with\ spaces.txt
    # This results in "file name with spaces.txt"

- We can also excape using double quotes instead of \ to ignore special characters
  that occur within the quotations.
- Single quotes will ignore ALL special characters and even SHELL VARIABLES.

- If a string includes double quotes somewhere between

Example:

    echo text"with middle"quotes
    > textwith middlequotes

Example:

    vim "file name with spaces.txt"
    vim 'why would you put "quotes" in a file name.txt'

The only exception is shell variables (certain words that begin iwth $)

Try the following:

    echo "this $PATH is expanded"
    echo 'this $PATH is not expanded'

**Even command names can be expanded if I have a program in the current directory
with a long name.**

If you want to refer to files relative to the home directory, we just use "~"

Example:

    echo ~

There are 2 standard files within the UNIX environment: stdin, stdout, stderr
- input: stdin
- output: stdout, stderr

Normally these are inherited from the parent process.

What this means that if I run a process from a shell, a program will have th esame input/output as the shell. (you can execute a C file from VSCODE or the console).

Sometimes we want to change this behavior.

Option 1) File redirection:

    program < input_file
        # specifies that the input_file shoul dbe use for stdin.
        # What this means is scanf will take in input from a file
        # as opposed to the console.

    program > output_file
        # Similarly, this specifies that the output_file should be used for std_out.
        # What this means is that printf will print output to a file
        # as opposed to the console.

    program 2> errfile
        # You guessed it, errors from program2 get printed into this file.

    
    Try:

        echo hello > f
        cat f

The reason we care about this pipes.

Pipes allow us to run two processes simultaneously after one another ( a pipeline),
which means that the stdout of one process becomes the stdin of another!

Example: Suppose you want to sort the contents several c files.

cat *.c | sort

The c files are put into the buffer, sort executes on them as they get fed in.

- We can use unique to specify that if two simultanous instances of the same line occur, only print one.

Examples:

    cat some_text | sort | uniq -c

    grep ERROR program.log | sort | uniq -c > error_counts.txt

Why do we have standard output and standard error. If a pipeline reads from standard output,
then it may not get parsed as an issue, hence why we have a separate output funnel to
specifically handle errors. We do not want to intermingle our valid output and erroneous output.

Every program is a process that handles input and outputs, so if errors were sent
to the output stream we would have big issues.

**Typing CTRL-C (^C) sends an interrupt to all current foreground running jobs (generally speaking this terminates the job)**.

^Z: hits pause on the current job (does not neccesarily terminate it.) and returns you
to the shell.

With a paused process you can:

- fg: to resume the most recently suspended job.
- bg: resumes a job, but in the background.
- kill: destroys, kills, demolishes, obliterates, the job.

To run a job in the background from the get gom put an "&" at the end of the command.

Example:

    long_running_job > output.txt &

To see current running jobs:

    jobs
    # Processes have nothing to the file system, so running this program
    # from any directory will result in the same result.

    ps
    # Prints currently running processes

    ps -e
    # Shows all running processes on the computer (there will be a lot).

    ps -f
    # Shows you the full command line history.

    ps -ef 
    # Shows every process and every command that was used to start it.

    top
    # Shows a live list of "top" processes


Environment variable:

    $?: - The exit code of the most recent process / job.

Try:

    echo $?




