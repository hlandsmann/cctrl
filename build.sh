#!/bin/bash -e

function create_build_dir() {
    if [ ! -d build/ ]
    then
        mkdir build
    fi
}
function build_project() {
    cd build
    if [ ! -f "Makefile" ]
    then
        cmake ..
    fi
    make
    avr-size --format=berkeley cctrl.elf
    cd ..
}

create_build_dir
build_project
