#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct
{
    char *word;
    int frequency;
} WordInfo;

WordInfo *words = NULL;
int numWords = 0;

void processFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        // Convert the word to lowercase for case-insensitive matching
        char *word = strdup(buffer);
        for (int i = 0; word[i]; i++)
        {
            word[i] = tolower(word[i]);
        }

        // Check if the word is already in the array
        int found = 0;
        for (int i = 0; i < numWords; i++)
        {
            if (strcmp(word, words[i].word) == 0)
            {
                words[i].frequency++;
                found = 1;
                free(word);
                break;
            }
        }

        // If the word is not found, add it to the array
        if (!found)
        {
            words = (WordInfo *)realloc(words, (numWords + 1) * sizeof(WordInfo));
            words[numWords].word = word;
            words[numWords].frequency = 1;
            numWords++;
        }
    }

    fclose(file);
}

void traverseDirectory(const char *path)
{
    struct dirent *entry;
    struct stat statbuf;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        char filepath[1024];
        sprintf(filepath, "%s/%s", path, entry->d_name);

        if (stat(filepath, &statbuf) == 0)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                {
                    // Recursively process subdirectories
                    traverseDirectory(filepath);
                }
            }
            else if (S_ISREG(statbuf.st_mode))
            {
                // Process regular files
                if (strstr(entry->d_name, ".txt") != NULL)
                {
                    // Process the text file and update word frequency
                    processFile(filepath);
                }
            }
        }
    }

    closedir(dir);
}

int compareWords(const void *a, const void *b)
{
    return ((WordInfo *)b)->frequency - ((WordInfo *)a)->frequency;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    const char *directory_path = argv[1];
    traverseDirectory(directory_path);

    // Sort the word frequency array
    qsort(words, numWords, sizeof(WordInfo), compareWords);

    // Print the sorted word frequency list
    for (int i = 0; i < numWords; i++)
    {
        printf("%s: %d\n", words[i].word, words[i].frequency);
        free(words[i].word);
    }
    free(words);

    return 0;
}
