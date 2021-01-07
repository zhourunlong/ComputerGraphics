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
#bin/RT testcases/cornellbox1.xml output/cornellbox1.bmp
#bin/RT testcases/smallpt.xml output/smallpt.bmp
#bin/RT testcases/coffee/scene.xml output/coffee.bmp
#bin/RT testcases/kitchen/scene.xml output/kitchen.bmp
#bin/RT testcases/spaceship/scene.xml output/spaceship.bmp
bin/RT testcases/cornellbox2.xml output/cornellbox2.bmp