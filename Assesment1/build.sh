#!/bin/bash

# Check if a file argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_file.c>"
    exit 1
fi

# Get the input filename
filename="$1"

# Remove the .c extension to create the output filename
output="${filename%.c}"

echo "Compiling $filename..."
gcc "$filename" -o "$output"

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful!"
echo ""
echo "Executable file is: $output"
