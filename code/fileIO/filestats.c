#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    struct stat file_stat;

    if (stat(path, &file_stat) == -1)
    {
        perror("stat");
        return 1;
    }

    printf("File or Directory: %s\n", path);
    printf("Inode number: %lu\n", (unsigned long)file_stat.st_ino);
    printf("File size: %lld bytes\n", (long long)file_stat.st_size);

    if (S_ISDIR(file_stat.st_mode))
    {
        printf("Type: Directory\n");
    }
    else if (S_ISREG(file_stat.st_mode))
    {
        printf("Type: Regular File\n");
    }
    else
    {
        printf("Type: Unknown\n");
    }

    printf("Mode (Permissions): %o\n", file_stat.st_mode & 0777);
    printf("Owner UID: %u\n", file_stat.st_uid);
    printf("Owner GID: %u\n", file_stat.st_gid);
    printf("Device ID: %ld\n", (long)file_stat.st_dev);
    printf("Number of hard links: %lu\n", (unsigned long)file_stat.st_nlink);
    printf("Block size: %lu bytes\n", (unsigned long)file_stat.st_blksize);
    printf("Blocks allocated: %lu\n", (unsigned long)file_stat.st_blocks);

    return 0;
}
