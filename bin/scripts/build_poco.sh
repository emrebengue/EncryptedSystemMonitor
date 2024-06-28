#!/bin/bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )"

PROJECT_ROOT="${SCRIPT_DIR}/../.."

# Define the Poco library directory
POCO_DIR="${PROJECT_ROOT}/lib/local/poco"

clone_poco(){
    rm -rf "${POCO_DIR}"
    git clone https://github.com/pocoproject/poco.git "${POCO_DIR}"
}


if [ ! -d "${POCO_DIR}" ] || [ ! -f "${POCO_DIR}/CMakeLists.txt" ]; then
    echo "cloning poco repository inside ${PROJECT_ROOT}/lib/local/poco"
    clone_poco
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
