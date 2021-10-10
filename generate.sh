#!/usr/bin/env -S bash -e
path_to_srcipt="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
workspace=$(cd $path_to_srcipt;pwd)
cd $workspace

build_dir=$workspace/build
mkdir -p $build_dir

toolchain_file=$workspace/cmake/toolchain_avr_m328p.cmake
#mode="Release"
mode="Debug"
cmake -B $build_dir \
    -DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
    -DCMAKE_BUILD_TYPE=$mode
(cd $build_dir; make -j$(nproc))
#(cd $build_dir; avr-size --format=berkeley cctrl.elf)