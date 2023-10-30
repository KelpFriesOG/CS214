# Recitation 5
### 1. Operations on Tar Archive Files
   - **Name and Working Principle**
   
     - **Tar** (Tape Archive) is a UNIX utility designed to package multiple files and directories into a single file. Tar is not only used for backup and archiving but is often used in conjunction with compression tools such as gzip and bzip2.

     - **Working Principle**: The Tar command converts multiple files and directories into a single file, a process known as packaging or archiving. During archiving, there is an option to compress the collection of files. Tar supports various compression formats, including `.gz`, `.bz2`, and `.xz`.
     **Note that** `zip` compresses individual files in the collection and archive them into a single file, while `tar` itself just packs everthing into a single file without compressing. `tar` only will compress the whole archive file with compress flags.

   - **Creating a Tar Archive File**
     - Explanation: Use the `tar` command to package and compress files and folders.
     - Example:
       ```bash
       tar -cvzf archive.tar.gz file1 file2 folder1
       ```
       This command creates an archive file named "archive.tar.gz" containing "file1", "file2", and "folder1".
       
       -c: Create a new archive.
       -v: Verbose mode (optional, it displays the files as they are added to the archive).
       -z: Compress the archive using gzip.
       -f: Specify the archive file's name.

   - **Extracting a Tar Archive File**
     - Explanation: Decompress a tar archive file to a specified directory.
     - Example:
       ```bash
       tar -xvf archive.tar.gz -C /path/to/directory
       ```
       This command extracts the files from "archive.tar.gz" to the specified directory.

       -x: Extract files from an archive.
       -v: Verbose mode (optional, displays the names of files as they are extracted).
       -f: Specify the archive file's name.
       -C: Change to DIR before performing any operations.  This option is order-sensitive, i.e. it affects all options that follow.
       
### 2. Permission Management

File permissions are composed of three parts: owner permissions, group permissions, and other user permissions. Each part has three types of permissions: read, write, and execute.

For example, the permissions of a file represented as `-rwxr-xr--`:
- `-`  The first character indicates the file type. In this case, it's a regular file. If it were a directory, it would be `d`
- `rwx` - The file owner has read, write, and execute permissions.
- `r-x` - The file group has read and execute permissions but not write permission.
- `r--` - Other users have read permission, without write and execute permissions.

#### Changing Permissions with `chmod`

1. **Change Permissions Using Alphabetic Notation**

   - **Add Execute Permission to the Owner**
     ```bash
     chmod u+x filename
     ```
     If the original permission was `rw-r--r--`, the new permission will be `rwxr--r--`.

   - **Remove Write Permission from the Group**
     ```bash
     chmod g-w filename
     ```
     If the original permission was `rwxrwxrwx`, the new permission will be `rwxr-xrwx`.

   - **Add Read Permission to Others**
     ```bash
     chmod o+r filename
     ```
     If the original permission was `rwxr-x---`, the new permission will be `rwxr-x--r`.

2. **Change Permissions Using Numeric Notation**
    The first digit is for owner permissions, the second digit is for group permissions, and the third is for other users. Each permission has a numeric value assigned to it: r (read): 4, w (write): 2, x (execute): 1

   - **Set Read, Write, and Execute Permissions for Owner, Group, and Others**
     ```bash
     chmod 777 filename
     ```
     The permission will be set to `rwxrwxrwx`.

   - **Set Owner to Read, Write, and Execute Permission; Group to Read and Execute Permission; Others to Read Permission**
     ```bash
     chmod 754 filename
     ```
     The permission will be set to `rwxr-xr--`.

#### Practical Example

Assume there is a file named `example.txt` with initial permissions `rw-r--r--`:

- The owner can read and write the file but cannot execute it.
- The group users and other users can only read the file.

We can use the `chmod` command to change these permissions:

```bash
chmod u+x example.txt  # Add execute permission to the owner
chmod g+w example.txt  # Add write permission to the group users
chmod o-w example.txt  # Remove write permission from other users (though they initially don't have write permission)
```

The updated permissions will be `rwxrw-r--`.

- The owner can read, write, and execute the file.
- The group users can read and write the file but cannot execute it.
- Other users can only read the file.

### 3. File Operations in C Language


#### Example 1: Using Predefined File Descriptors

In UNIX and Linux systems, a file descriptor is a non-negative integer that represents the way processes access files, pipes, and network sockets. Here are some common file descriptors:

- 0 - Standard Input (stdin)
- 1 - Standard Output (stdout)
- 2 - Standard Error (stderr)
- 3 - New file (In C language)

When you open a file, the system returns a new file descriptor, typically the smallest unused integer.

**Working Principle**: The `open` function returns a file descriptor representing the opened file. The `read` and `write` functions access the file through the file descriptor. The `close` function closes the opened file descriptor, freeing up resources.

```c
#include <unistd.h>

int main() {
    // Write data to standard output
    write(1, "Hello, World!\n", 14);

    // Write error message to standard error
    write(2, "An error occurred.\n", 19);

    return 0;
}
```

#### Example 2: Reading and Writing Files Using File Descriptors

Here is a C language example demonstrating how to use file descriptors to open, read, write, and close files.

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Cannot open");
        return 1;
    }

    // Write data to the file
    const char *data = "Hello, World!\n";
    ssize_t written = write(fd, data, 14);
    if (written == -1) {
        perror("Cannot write");
        close(fd);
        return 1;
    }

    // Move the file position pointer to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    // Read data from the file
    char buffer[1024];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("Cannot read");
        close(fd);
        return 1;
    }
    buffer[bytesRead] = '\0';  // Add the string termination character

    // Print the read data
    printf("%s", buffer);

    // Close the file descriptor
    close(fd);

    return 0;
}
```

In this example:

- The `fcntl.h` header is for `file control options`
- The `open` function is used to open a file and returns a file descriptor `fd`.
- The `write` function uses this file descriptor to write data to the file.
- The `lseek` function is used to move the file position pointer.
- The `read` function uses the same file descriptor to read data from the file.
- The `close` function is used to close the file descriptor and release the resources.
- The `perror` function takes a pointer to a null-terminated string, typically containing a user-defined error message. It's followed by a colon and a space, then followed by a textual representation of the system error information.

**The file descriptor increments for each created
file.**

#### perror and stderr

- **Returning `-1`**: In the context of system calls (such as `read`, `write`, `open`, etc.) and certain library functions, `-1` typically indicates a failure of the operation. In such cases, the `perror` function can be used or the `errno` variable can be checked to obtain details about the error.

- **File Descriptor `2`**: This is the file descriptor for standard error (stderr). Every UNIX or Linux process starts with three default file descriptors: `0` for standard input, `1` for standard output, and `2` for standard error.

For instance, in the following code:

```c
ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
if (bytesRead == -1) {
    // Here, -1 indicates that the read function failed
    perror("read");
    // perror will write the error message to file descriptor 2, i.e., standard error
}
```

If the `read` function fails, `bytesRead` will be `-1`, and the error can be identified and reported using `perror` or `errno`. The error message will be sent to file descriptor `2` (i.e., standard error).

