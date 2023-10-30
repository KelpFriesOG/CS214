#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to determine the file type based on the file extension
const char *getFileType(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return "Unknown";
    }

    // List of common file extensions and their corresponding file types
    struct {
        const char *ext;
        const char *type;
    } fileTypes[] = {
        {".txt", "Text File"},
        {".c", "C Source Code"},
        {".h", "C Header File"},
        {".cpp", "C++ Source Code"},
        {".jpg", "JPEG Image"},
        {".png", "PNG Image"},
        {".pdf", "PDF Document"},
        {".html", "HTML Document"},
        {".js", "JavaScript Code"},
        {".py", "Python Script"},
        {".sh", "Shell Script"},
        // Add more file extensions and types as needed
    };

    for (size_t i = 0; i < sizeof(fileTypes) / sizeof(fileTypes[0]); i++) {
        if (strcmp(dot, fileTypes[i].ext) == 0) {
            return fileTypes[i].type;
        }
    }

    return "Unknown";
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int character_count = 0;
    int word_count = 0;
    int line_count = 0;

    int in_word = 0;
    char c;

    while ((c = fgetc(file)) != EOF) {
        character_count++;

        // Check for word and line boundaries
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            in_word = 0;
            if (c == '\n') {
                line_count++;
            }
        } else {
            if (!in_word) {
                in_word = 1;
                word_count++;
            }
        }
    }

    // Close the file
    fclose(file);

    // Determine the file type
    const char *fileType = getFileType(argv[1]);

    // Print statistics and file type
    printf("File: %s\n", argv[1]);
    printf("File Size: %d bytes\n", character_count);
    printf("Number of Words: %d\n", word_count);
    printf("Number of Lines: %d\n", line_count);
    printf("File Type: %s\n", fileType);

    return 0;
}

