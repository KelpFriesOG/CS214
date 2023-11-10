Typically we use fork(), exec(), and wait() in tandem.

We use fork() to create a new process, and wait() to wait for that process to finish.

wait() returns a p_id structure that contains information about the child process

```

pid_t wait(int *wstatus);

Returns the PID of the child that terminated.

- If the child has not terminated, it blocks until it does.

- If more than one child has already
terminated, it returns immeadiately.

- If more than one child has already terminated, no order is assumed.

```

- wait() waits for the child to change state (this includes stopping and resuming which are different from termination).

- **To tell what happened, check the value written to wstatus.**

WIFEXITED will tell you if the child exited normally. (returns true)

- If it exited normally, you can check the exit status with WEXITSTATUS.

If it exited abnormally (i.e. killed) WIFSIGNAL will be true.

- If it exited abnormally, you can check the exit status with WTERMSIG.

**WCOREDUMP** tell you if we crashed.

We can use the fg and bg process to continue a stopped process.

- When the process is resumed again, wait() will return again but
  WIFCONTINUE will be true. (WCONTINUE is true if the stopped process
  has been unpaused)

- 0 means success, non-zero means failure.

- Note the function psignal

```


```

## Communication between the parent and the child
---

We have seen that exit and wait can send 1 byte of data from the child to
the parent.

We can also use exec() to pass strings from the parent to the child.
- The child has access to all the data from the point it was called.

- We can put information for the child in the argument list or the environment.

The environment is a list of key-value pairs.

- We can store some data in the environment.

- Open files can be passed to a subprocess (file descriptors carry over).

*The SIGNALS are basically bit vectors (bit manipulation operations)*

When you have a file open in both the parent and the child, what the hell happens?

- The file position is stored by the open file record which is kept by the OS,
  this means updating the open file position is one process affects it in another.

- This is how the parent and child can print to the same terminal.

---

## The pipe() function
---

This function creates two file descriptors that point to the same object.

```

int pipe(int fd[2]);
// Returns 0 on success, -1 on failure.

int fd[2];
pipe(fd);

```

You write into index 1, you read from index 0.

- The pipe function creates a FIFO queue.
- Each end of the "pipe" has an associated file descriptor.
- The read end is fd[1] and the write end is fd[0].

int fd[2];
int r = pip(fd);

One process writes to the pipe the other process reads to the pipe.


**We want to run a program in our child process which will write to stdout (fd = 1)**

**We want the parent to recieve this data.**

**You could create a pipe but you'd get a new fd**

So what is the plan?:

We want the parent to recieve the child process's output
we can create a pipe, but how can we tell the child to write to it?

Lets take a look at the dup() function.

---

## The dup() function

You give it a file descriptor for a currently open file, it returns
a new file descriptor that refers to the same file!

int dup(int fd):
    Creates a new file descriptor that refers to the same file as fd.
    - The descriptors can be used interchangeably.

int dup2(int oldfd, int newfd):
    Like dup(), but we specify the file descriptor for the duplicate.
    -> If the descriptor that is passed is already being used, it is closed.

```

int fd[2]
pipe(fd);

if(fork() == 0){

    dup2(fd[1], STDOUT_FILENO);
    // optionally
    // close(fd[0]);
    // close(fd[1]);
    
    // Nothing goes wrong if we don't close fd[1] here.
    // Nothing really goes wrong if we don't close fd[0] here.
    // But you might as well.

    // If this works, all subsequent writes to stdout will go to the pipe.

    execl(...);
    // If we get here, something is wrong.
}

// The parent can read the child's output from fd[0].

while((bytes = read(fd[0], buf, AMOUNT)) > 0){
    // Obtain data from the child process.
}
// When we finish using the pipe, we have to close the reference:

close(fd[1]);
// If we don't close fd[1] in the parent, we will never get EOF.

// At some point there will be a situation where
// no more data could be written to the pipe.

```

**read end is first, write end is second**

After line dup2(), there are three file descriptors that refer to the write
end of the pipe.

- Reading from any of the open file descriptors does not mean you have
  3 separate cursors on the file. (this is not like calling open on the same file twice).

- If I write to any one of these fds, all file descriptors will update
  to write the next portion later in.

By the time the child process terminates, will the write in be closed?

By closing one file descriptor, the file does not close until all 
file descriptors that refer to it close!

We can use pipe() and dup2() to establish communication between
parent and child using child's stdin, stdout, and/or stderr.

**Every reference to the file has to be closed.**

**When we execute a program in the shell we can specify where to read
and write from. How do they do it?**

- PIPES!
- DUP2!
- EXEC!

The shell basically does:

```

// It opens a file, it changes the file descriptor, and it executes a program.
// Input redirection!!!

if (fork() == 0){

    int fd = open(somefile, O_RDONLY);
    dup2(fd, STDIN_FILENO);
    execl(some_program, ..., ...., NULL);
}

```

How do I open two processes and establish communication?
- check errcodes in your own implementation.

```

// Start both processes.

int fd[2];
print(fd);

if (fork() == 0){
    // first child
    dup2(fd[1], STDOUT_FILENO);
    execl(first_program, ..., NULL);
    exit(1);
}

close(fd[1]);
close(fd[0]);

if (fork() == 0){
    // second child
    dup2(fd[0], STDIN_FILENO);
    execl(second_program, ..., NULL);
    exit(1);
}


```

---