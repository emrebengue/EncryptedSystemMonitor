#!/bin/bash

# Exit immediately if a command exits with a non-zero status, and treat unset variables as an error.
set -euo pipefail

#https://stackoverflow.com/questions/39340169/dir-cd-dirname-bash-source0-pwd-how-does-that-work

# Get the directory of the current script
# BASH_SOURCE is the name for the executing script. In this case it is monitorized_task_collection.sh
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define the project root based on the script's location
PROJECT_ROOT="${SCRIPT_DIR}/.."

echo "SCRIPT_DIR: ${SCRIPT_DIR}"
echo "PROJECT_ROOT: ${PROJECT_ROOT}"

cd "${PROJECT_ROOT}"

mkdir -p build
cd build

cmake ..
make

# Set the LD_LIBRARY_PATH environment variable to include the Poco library path
export LD_LIBRARY_PATH="${PROJECT_ROOT}/lib/local/poco/lib:${LD_LIBRARY_PATH:-}"
# Run the compiled program

cd "${PROJECT_ROOT}/compiled"

"${PROJECT_ROOT}/compiled/monitorTasks"

