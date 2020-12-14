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
bin/RT testcases/scene01_basic.txt output/scene01.bmp
bin/RT testcases/scene02_cube.txt output/scene02.bmp
bin/RT testcases/scene03_sphere.txt output/scene03.bmp
bin/RT testcases/scene04_axes.txt output/scene04.bmp
bin/RT testcases/scene05_bunny_200.txt output/scene05.bmp
bin/RT testcases/scene06_bunny_1k.txt output/scene06.bmp
bin/RT testcases/scene07_shine.txt output/scene07.bmp
#bin/RT testcases/scene08_bunny.txt output/scene08.bmp
