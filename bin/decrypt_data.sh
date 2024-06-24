#!/bin/bash

set -euo pipefail

DESKTOP_DIR="$HOME/Desktop"
DOCUMENTS_DIR="$HOME/Documents"

SHA256_PASSWORD="4cfc6666a27d7182247b565967e6b7476f81a62b5c338d694cbe9e929c1da7ff"

decrypt_file() {
    local file="$1"
    local decrypted_file="${file%.encrypt}"

    openssl aes-256-cbc -d -salt -in "$file" -out "$decrypted_file" -k "$SHA256_PASSWORD"

    if [[ $? -eq 0 ]]; then
        echo "$file decrypted successfully"
        rm "$file"
    else
        echo "$file decryption failed"
    fi
}

find "$DESKTOP_DIR" "$DOCUMENTS_DIR" -type f -name "*.encrypt" | while read -r file; do
    decrypt_file "$file"
done

echo "Decryption process completed"
