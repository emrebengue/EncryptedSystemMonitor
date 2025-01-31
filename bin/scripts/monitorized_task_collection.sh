#!/bin/bash

# Exit immediately if a command exits with a non-zero status, and treat unset variables as an error.
set -euo pipefail

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define the project root based on the script's location
PROJECT_ROOT="${SCRIPT_DIR}/../.."

#echo "SCRIPT_DIR: ${SCRIPT_DIR}"
#echo "PROJECT_ROOT: ${PROJECT_ROOT}"

# Define the Poco library directory
POCO_DIR="${PROJECT_ROOT}/lib/local/poco"

#"${SCRIPT_DIR}/build_poco.sh"

# Set the LD_LIBRARY_PATH environment variable to include the Poco library path
export LD_LIBRARY_PATH="${POCO_DIR}/lib:${LD_LIBRARY_PATH:-}"

# Run the compiled program
cd "${PROJECT_ROOT}/compiled"
"${PROJECT_ROOT}/compiled/monitorTasks"
