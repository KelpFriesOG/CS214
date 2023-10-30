#!/bin/bash

# Prompt the user for a filename
read -p "Enter the filename: " filename

# Check if the file exists
if [ ! -f "$filename" ]; then
    echo "File '$filename' not found."
    exit 1
fi

# Compile the C program
gcc -o file_stats stats.c

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."

    # Run the compiled program with the provided filename as an argument
    ./file_stats "$filename"
else
    echo "Compilation failed."
fi
