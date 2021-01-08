#!/usr/bin/env bash

rm /etc/apt/sources.list
cp sources.list /etc/apt/sources.list
apt update
apt upgrade
apt install -y clang-9 libc++-9-dev libc++abi-9-dev cmake ninja-build
apt install -y libz-dev libpng-dev libjpeg-dev

if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

cd build
make -j
cd ..

mkdir -p output
#bin/RT testcases/cornellbox.xml output/cornellbox.bmp
#bin/RT testcases/smallpt.xml output/smallpt.bmp
#bin/RT testcases/coffee/scene.xml output/coffee.bmp
#bin/RT testcases/kitchen/scene.xml output/kitchen.bmp
#bin/RT testcases/spaceship/scene.xml output/spaceship.bmp
bin/RT testcases/bathroom/scene.xml output/bathroom.bmp
