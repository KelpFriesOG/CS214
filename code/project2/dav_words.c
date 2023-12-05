#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_WORD_LEN 256

typedef struct word_count
{
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

void print_word_counts(WordCount *word_counts, int count);
int count_words(FILE *file, WordCount **word_counts);
void free_word_counts(WordCount *word_counts, int count);

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s file...\n", argv[0]);
        return 1;
    }

    WordCount *word_counts = NULL;
    int count = 0;

    for (int i = 1; i < argc; i++)
    {
        struct stat sb;
        if (stat(argv[i], &sb) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(sb.st_mode))
        {
            DIR *dir = opendir(argv[i]);
            if (dir == NULL)
            {
                perror("opendir");
                continue;
            }

            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL)
            {
                if (strlen(entry->d_name) >= 4 &&
                    strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0)
                {
                    char path[1024];
                    snprintf(path, sizeof(path), "%s/%s", argv[i], entry->d_name);
                    FILE *file = fopen(path, "r");
                    if (file == NULL)
                    {
                        perror("fopen");
                        continue;
                    }

                    count_words(file, &word_counts);
                    fclose(file);
                }
            }

            closedir(dir);
        }
        else
        {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL)
            {
                perror("fopen");
                continue;
            }

            count_words(file, &word_counts);
            fclose(file);
        }
    }

    print_word_counts(word_counts, count);
    free_word_counts(word_counts, count);


    return 0;
}

void print_word_counts(WordCount *word_counts, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int count_words(FILE *file, WordCount **word_counts)
{
    int count = 0;
    char word[MAX_WORD_LEN];
    int word_len = 0;

    while (1)
    {
        int c = fgetc(file);
        if (c == EOF)
        {
            break;
        }

        if (isalnum(c) || c == '-' || c == '\'')
        {
            word[word_len++] = tolower(c);
        }
        else
        {
            if (word_len > 0)
            {
                word[word_len] = '\0';
                int found = 0;

                for (int i = 0; i < count; i++)
                {
                    if (strcmp((*word_counts)[i].word, word) == 0)
                    {
                        (*word_counts)[i].count++;
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    *word_counts = realloc(*word_counts, (count + 1) * sizeof(WordCount));
                    strcpy((*word_counts)[count].word, word);
                    (*word_counts)[count].count = 1;
                    count++;
                }

                word_len = 0;
            }
        }
    }

    return count;
}

void free_word_counts(WordCount *word_counts, int count)
{
    free(word_counts);
}