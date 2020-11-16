#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# Build project.
cd build
make -j
cd ..

# Run all testcases. 
# You can comment some lines to disable the run of specific examples.
mkdir -p output
bin/PA1 testcases/family.bmp output/family.bmp 1 0.3
bin/PA1 testcases/Fatem.bmp output/Fatem.bmp 1 0.3
bin/PA1 testcases/foliage.bmp output/foliage.bmp 0 0.3
bin/PA1 testcases/waterfall.bmp output/waterfall.bmp 1 0.5
