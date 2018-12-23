#!/bin/bash
#set -x

[ ! -d build ] && { mkdir build; }
cd build || exit 1
cmake .. || exit 1
make -j || exit 1
