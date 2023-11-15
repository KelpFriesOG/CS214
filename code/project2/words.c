#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define BUFSIZE 1024

/**word.c takes a directory and sorts through it finding all text files under it
 * or other directories inside of it. It creates a linked lists of all the words
 * found within the text files, along with a running frequency of the word being
 * found.
 *
 * The final output is to print a list of all the words found within all the text
 * files, along with the frequency (times the word was found).
 *
 * @Kalpesh Chavan, Hieu Nguyen
 */

/**WORD represents a word, its frequency, and a link to the next word in the linked list
 */
typedef struct Word
{
    char *word;
    int frequency;
    struct Word *next;
} WORD;

/**Dictionary creates a linked list of WORD
 *
 */
typedef struct Dictionary
{
    WORD *head;
    int size;
} DICT;

/* Prototypes for used functions */
int is_letter(char c);
void print_dict(DICT *dict);
void make_unique_frequencies(DICT *dict, int unique_freq[]);
void add_word(DICT *dict, char *word);

/**checks if given char is a valid letter or symbol
 *
 * checks if c is an A to Z letter, upper and lower case
 * checks if c is an apostrophe
 *
 * @param c the char to be checked
 * @return 1 if the char is upper or lower A to Z letter, or apostrophe
 * @return 0 if char is not valid char
 */
int is_letter(char c)
{
    // Letters include letters and apostrophe
    // If the character is a letter return 1
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return 1;
    }

    // If a character is a ', return 1
    if (c == '\'')
    {
        return 2;
    }

    return 0;
}

/**processes a text file and modifies dictionary with the words in it
 *
 * opens a file and reads char by char
 * checks the condition for word by the current char's following char
 * adds word if condition is met
 * continues until the end of the text file
 *
 * @param *filePath the path to the text file in directories
 * @param *dict the dictionary containing words and frequencies
 *
 */
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

    while ((bytesRead = read(fd, buffer, BUFSIZE)) > 0)
    {
        for (ssize_t i = 0; i < bytesRead; i++)
        {
            if (is_letter(buffer[i]))
            {
                word[wordIndex++] = buffer[i];
            }
            else if (wordIndex > 0 && buffer[i] == '-' && i < bytesRead - 1 && is_letter(buffer[i + 1]) == 1)
            {
                word[wordIndex++] = buffer[i];
                // butter-fly
                // butter-----fly
            }
            else if (wordIndex > 0)
            {
                word[wordIndex] = '\0';
                add_word(dict, word);
                wordIndex = 0;
            }
        }

        // Deal with and add any leftover word inside the word array
        if (wordIndex > 0)
        {
            word[wordIndex] = '\0';
            add_word(dict, word);
            wordIndex = 0;
        }
    }

    if (wordIndex > 0)
    {
        word[wordIndex] = '\0';
    }

    

    close(fd);
}

/**Adds new word or increases frequency count in dictionary
 *
 * takes an input word and searches through dictionary
 * if word is found, increase its count by 1
 * if word is not found, add it as a new word to dictionary
 *
 * @param *dict the dictionary containing words and frequencies
 * @param *word the word to be added or modified in dictionary
 * @return modifies dictionary to either add new word or increase a word count
 *
 */
void add_word(DICT *dict, char *word)
{

    // Try to find the word in the dictionary
    WORD *current = dict->head;

    if (dict->head == NULL)
    {
        dict->head = (WORD *)malloc(sizeof(WORD));
        dict->head->word = strdup(word);
        dict->size = 1;
        dict->head->frequency = 1;
        dict->head->next = NULL;
        return;
    }

    // If the word is already in the dictionary
    // just increment the count associated with it.
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
    dict->size = dict->size + 1;

    // Search through dict to find the correct position
    // for the new word
    WORD *prev = NULL;
    current = dict->head;

    // If the new word comes lexographically before the current->word
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

    return;
}

/**Finds and sorts all unique frequencies of words in dictionary
 *
 * First logs all of the frequencies in array
 * sorts the entire array in decreasing order via bubble sort
 * modifies uniq_freqs to contain only unique values in descending order
 *
 * @param *dict the dictionary of words and frequencies
 * @param unique_freq the to-be modified array with all unique frequencies in dict
 * @return modifies unique_freq to contain all unique frequencies of words in dict in descending order
 *
 */
void make_unique_frequencies(DICT *dict, int unique_freq[])
{
    WORD *current = dict->head;
    int size = dict->size;
    int freqs[size];

    // fills array with all feqs
    for (int i = 0; i < size; i++, current = current->next)
    {
        freqs[current->frequency]++;
    }

    // sorts the array of all freqs in descending
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (freqs[i] < freqs[j])
            {
                // Swap the elements
                int temp = freqs[i];
                freqs[i] = freqs[j];
                freqs[j] = temp;
            }
        }
    }

    // fills new array with only unique freqs
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        if (i == 0 || freqs[i] != freqs[i - 1])
        {
            unique_freq[index++] = freqs[i];
        }
    }
}

/**Prints the dictionary's words and frequencies
 * @param the dictionary of words found
 * @return prints the words inside dictionary based on frequency, then lexographic order
 */
void print_dict(DICT *dict)
{

    int unique_freq[dict->size];
    make_unique_frequencies(dict, unique_freq);

    int uniq_size = sizeof(unique_freq) / sizeof(unique_freq[0]);

    for (int i = uniq_size; i >= 0; i--)
    {
        WORD *runner = dict->head;
        while (runner != 0)
        {
            if (i == runner->frequency)
            {
                printf("%s: %d\n", runner->word, runner->frequency);
            }
            runner = runner->next;
        }
    }
}

/**Traverses and searches through directories for text files
 *
 * Uses a DFS system to find all directories and logs the text files
 *
 * @param *path takes the initial path directory
 * @param *words sta=rts the entire dictionary
 * @return void
 */
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

/**Initializes dictionary
 * @param starting dictionary
 * @return void
 *
 */
void init_dict(DICT *dict)
{
    dict->head = NULL;
}

void free_dict(DICT *dict)
{
    WORD *current = dict->head;
    while (current != NULL)
    {
        WORD *next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}

int main(int argc, char *argv[])
{

    const char *path;

    // Pickup path from second argument
    // Else return usage message
    if (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }

    DICT *words = (DICT *)malloc(sizeof(DICT));
    init_dict(words);

    traverse(path, words);

    print_dict(words);

    free_dict(words);

    free(words);

    return 0;
}