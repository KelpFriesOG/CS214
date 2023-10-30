typedef struct
{
    char *word;
    int frequency;
} WORD;

int compare_frequency(WORD *word1, WORD *word2);

void traverse_path(const char *path);

void process_file(const char *file);