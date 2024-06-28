#!/bin/bash

set -euo pipefail

./scripts/build_poco.sh
./scripts/monitorized_task_collection.sh
