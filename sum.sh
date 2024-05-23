#!/bin/bash

# Directory containing the output files
OUTPUT_DIR="outputs"
# File to store the consolidated results
CONSOLIDATED_FILE="consolidated_results.txt"

# Empty the consolidated file if it exists
>$CONSOLIDATED_FILE

# Iterate over each file in the output directory
for file in "$OUTPUT_DIR"/*_times.txt; do
    # Extract the file name without the directory path
    filename=$(basename "$file")

    # Read the line containing "Average time (excluding first run):" and append it to the consolidated file
    grep "Average time (excluding first run):" "$file" | while read -r line; do
        echo "$filename: $line" >>$CONSOLIDATED_FILE
    done
done

echo "Consolidation complete. Results stored in $CONSOLIDATED_FILE."
