#!/bin/bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )"

PROJECT_ROOT="${SCRIPT_DIR}/../.."

# Define the Poco library directory
POCO_DIR="${PROJECT_ROOT}/lib/local/poco"

if [ ! -d "${POCO_DIR}" ]; then
    git clone https://github.com/pocoproject/poco.git "${POCO_DIR}"
fi

cd "${POCO_DIR}"
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX="${POCO_DIR}" ..
make -j$(nproc)
make install

cd "${PROJECT_ROOT}"

rm -rf build

mkdir -p build
cd build

cmake ..
make
