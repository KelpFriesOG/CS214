# CS214 / 2023-10-18

## Project 1 Notes

---

Error messages should note that the file name and line number
- Ideally describe what went wrong.
- mymalloc() should return NULL on an error (beyond the print statement).
- Print an error message for allocating 0 bytes.

Free returns void, so if its given a bad pointer then the most
we can do is print an error message.

- **After printing the error message we can do nothing and return.**
- Alternatively we could halt the program by calling exit().
- Explain why we chose the choice we did in the readme.

Malloc and free have to detect if an array is uninitialized.
- If the first header has 0 for both entries (or if you do not allow for 0 size blocks, simply if size = 0)
  then the array must be uninitialized.
- **Try memtest.c from canvas**

---

## Manipulating Storage

Storage in our context is non-volatile meaning that it keeps data even without power.
- RAM is volatile "playground" memory that our system uses as temporary space to hold
  running programs.

The UNIX model generalizes the concept of a file i.e. "file access model"

- files on disk
- I/O from console
- I/O from programs
- I/O from networks
- I/O from arbitrary devices

Basically if something at the most fundamental level is simply a stream of bytes,
can be treated as a file. 

- Later on we will see how we use the file interface to treat network data as a file.
- A "full duplex" stream has both input and output functionality.

Technically we have two groups of file functions.

**C offers buffered file access**
- FILE *, fopen(), fclose(), fread(), fwrite(), fscanf(), fprintf()

Otherwise **POSIX provides low-level non-buffered file access functions**
- File descriptors (ints)
- open(), close(), read(), write(), lseek(), ...
- 0, 1, 2 (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)

---

## What is a buffer?

Everytime we want to read one byte we don't want to make individual system
calls for each byte.

- This is feasible, but inefficent and not practical.

Modern computers take in a big batch or buffer of data with every system call.

- So to read byte by byte for n bytes requiring n system calls, we make a single system
  call to retrieve n bytes and then read each byte from the buffer. 

- When reading from a file we process the file in batches to minimize
  the number of system calls we have to make.

- When writing to a file we write our changes into the buffer and commit
  all our changes once we are done.

---

## The FILE struct

The FILE struct contains a buffer and a cursor within the buffer.

- When reading a file we use a system call to fill the buffer and then read
 from the buffer and remove as we read from it until the buffer is empty.

- Then the next system call will refill the buffer.

- When writing to a file we write bytes into the buffer, when the buffer
  is full we send the buffer to the OS and then clear the buffer.

Three buffering strategies:

1) Regular files will wait until the buffer is full to send data.
2) stdout is "line buffered"; we flush when we write "\n"
3) stderr flushes after each write function

fflush() flushes the FILE's buffer.

The non-buffered functions simply use a buffer provided by the user.
Whatever is in the buffer is committed directly into the file based
on the position on the cursor.

---

## write() function

Ex:

    // fd is the descriptor for the file we are writing to.
    // buf is the pointe rto the buffer containing th e bytes to write.
    // bytes is the number of bytes to write

    int write( int fd, void *buf, size_t bytes)

    // write returns -1 if it encounters any critical problem
    // It also sets errno

- Data that you print does not technically make it to its destination
  until you flush the buffer.


Try understanding the following functions in your own time: perror() and strerror().

- These functions utilize the errno to return a more detailed and verbose error
  as opposed to a catch all -1 return value or an meaningless errno value.

The C functions take care of the buffer and its size and whatnot,
with POSIX functions you have to customize the buffer and deal with it.
- **Internally the C functions utilize POSIX functions**

---

## read() function

Ex:

    size_t read( // number of bytes written to the buffer
        int fildes, // descriptor of file to read from
        vod *buf, // address to write bytes to
        size_t n_byte // length of buffer
        )

On error, the function returns -1 and sets errno
- If read() returns 0, then it means we are at the end of the file.

Keep in mind that read() is type agnostic. It does not put a terminator
at the end of files or its buffer and it simply copies n bytes.

If you tell read to read 0 bytes, it will happily do so.

The type of the data we read is simply bytes, if we know they are
integers then we interpret them appropriately.

- We know that the file ends if read() returns 0, so we could try this
  with a while loop.

Ex:

    char *buf[BUFSIZE];
    int bytes;

    bytes = read(fd, buf, BUFSIZE);
    total_bytes = bytes;

    while(bytes > 0){
        do stuff with buffer
        ...
        bytes = read(fd, buf, BUFSIZE);
        total_bytes += bytes;
    }

Alternatively:

    char *buf[BUFSIZE];
    int bytes;

    total_bytes = bytes;

    while(bytes = read(fd, buf, BUFSIZE) > 0){
        
        for(i = 0; i < bytes; i++){
            do stuff with buffer
            ...
        }

        total_bytes += bytes;
    }

---

## open() function

Ex:

    int open( // Returns a file descriptor on success, otherwise returns -1 and sets errno
        char* path,
        int flags
    )

We use the flags to tell the compiler what we want to do with the file
(reminds me of Python file i/o). 

Here are some common flags:

    - O_RDONLY - open file for reading
    - O_WRONLY - open file for writing
    - O_RDRW - open file for both

    - O_CREAT - create file if it does not already exist ***

By default we overwrite the contents of the file, but what if
we want to empty the file before writing to it or append to it's
pre-existing contents?

    - O_TRUNC - set file length to 0 if it already exists
    - O_APPEND - anytime we write initialize the cursor to the end of the file
    - O_EXCL - fail O_CREAT if the file already exists.

Example:

    outfile = open(my_output_file_path, 0_WRONLY | O_CREAT | O_TRUNC, permissions)

    infile = open(my_input_file_path, O_RDONLY);

    if (infile == -1){
        perror(my_input_file_path);
        do something .....
    }

---

## UNIX File Permissions

Every file in UNIX has a designated owner and group.

- We can set the permissions for a file's owner, a particular group, or others.
- What this means is that there are three categories: owner, the group, and others
  which can set to have any possible combination of permissions.

- Keep in mind that the execute permission is separate from the read permission.

- So you cannot necessarily execute a file even if you can read it.

Each permission is a bit: three groups -> three bits -> octal

Example:

u   g   o
rwx rwx rwx
110 100 100



