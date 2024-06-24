#!/bin/bash

# Exit immediately if a command exits with a non-zero status, treat unset variables as an error, and prevent errors in pipelines
set -euo pipefail

# Define the directories
DESKTOP_DIR="$HOME/Desktop"
DOCUMENTS_DIR="$HOME/Documents"

# Function to delete encrypted files
delete_encrypted_files() {
    find "$DESKTOP_DIR" "$DOCUMENTS_DIR" -type f -name "*.encrypt" -exec rm -f {} +
}

# Call the function to delete the files
delete_encrypted_files

echo "All .encrypt files in Desktop and Documents have been deleted."
