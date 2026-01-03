#!/bin/bash
# source .env, getenv()
if [ -f .env ]; then
    export $(grep -v '^#' .env | xargs)
fi

# Create build directory and compile the project
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run the compiled binary
./build/smarthome_ci
