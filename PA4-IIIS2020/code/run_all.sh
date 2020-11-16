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
bin/PA4 testcases/scene06_bunny_1k.txt output/scene06.bmp
bin/PA4 testcases/scene11_bunny_ffd.txt output/scene11.bmp
bin/PA4 testcases/scene12_cylinder_ffd.txt output/scene12.bmp
bin/PA4 testcases/scene13_fish_ffd.txt output/scene13.bmp
