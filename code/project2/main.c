#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define BUFSIZE 1024

// Create a struct to hold the word and its frequency
typedef struct Word
{
    char *word;
    int frequency;
    struct Word *next;
} WORD;

typedef struct Dictionary
{
    WORD *head;
} DICT;

int is_letter(char c)
{
    // Letters include letters and apostrophe
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '\'');
}

void process_file(const char *filePath, DICT *dict)
{
    int fd = open(filePath, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    char buffer[BUFSIZE];
    ssize_t bytesRead;
    char word[BUFSIZE];
    int wordIndex = 0;

    printf("Words in %s:\n", filePath);
    while ((bytesRead = read(fd, buffer, BUFSIZE)) > 0)
    {
        for (ssize_t i = 0; i < bytesRead; i++)
        {
            if (is_letter(buffer[i]))
            {
                word[wordIndex++] = buffer[i];
            }
            else if (wordIndex > 0)
            {
                word[wordIndex] = '\0';
                printf("%s\n", word);
                // Add the word to the dictionary
                add_word(dict, word);
                wordIndex = 0;
            }
        }
    }

    if (wordIndex > 0)
    {
        word[wordIndex] = '\0';
        printf("%s\n", word);
    }

    close(fd);
}

void add_word(DICT *dict, char *word)
{

    // Try to find the word in the dictionary
    WORD *current = dict->head;

    // If dict is empty, make word the head
    if (dict->head == NULL)
    {
        dict->head = (WORD *)malloc(sizeof(WORD));
        dict->head->word = strdup(word);
        dict->head->frequency = 1;
        dict->head->next = NULL;
        return;
    }

    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->frequency++;
            return;
        }
        current = current->next;
    }

    // If you could not find the word, add it to the dictionary
    // in the correct position based on lexographical order
    WORD *newWord = (WORD *)malloc(sizeof(WORD));
    newWord->word = strdup(word);
    newWord->frequency = 1;

    // Search through dict to find the correct position
    // for the new word
    WORD *prev = NULL;
    current = dict->head;

    while (current != NULL && strcmp(current->word, word) < 0)
    {
        prev = current;
        current = current->next;
    }

    if (prev == NULL)
    {
        dict->head = newWord;
    }

    else
    {
        prev->next = newWord;
    }

    newWord->next = current;

    // Return with the changed dictionary
    return;
}

void print_dict(DICT *dict)
{
    WORD *current = dict->head;
    while (current != NULL)
    {
        printf("%s: %d\n", current->word, current->frequency);
        current = current->next;
    }
}

void traverse(const char *path, DICT *words)
{

    // If the stat function fails, print an error message and return
    struct stat path_stat;
    if (stat(path, &path_stat) == -1)
    {
        perror("lstat");
        return;
    }

    // If the path is a directory, traverse its contents
    if (S_ISDIR(path_stat.st_mode))
    {
        DIR *dir = opendir(path);
        if (!dir)
        {
            perror("Error opening directory");
            return;
        }

        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL)
        {
            char childPath[1024];
            snprintf(childPath, sizeof(childPath), "%s/%s", path, entry->d_name);

            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                traverse(childPath, words);
            }
        }

        closedir(dir);
    }

    // Otherwise, process the file only if it's a text file
    else if (S_ISREG(path_stat.st_mode))
    {
        if (strstr(path, ".txt") != NULL)
        {
            process_file(path, words);
        }
    }
}

void init_dict(DICT *dict)
{
    dict->head = NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <path_to_directory_or_file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    DICT *words = (DICT *)malloc(sizeof(DICT));
    init_dict(words);

    traverse(path, words);

    print_dict(words);

    return 0;
}
