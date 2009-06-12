#!/bin/bash

if [ -x ./autogen.sh ]; then
    ./autogen.sh CFLAGS="-Wall -Wextra -g0 -O3"
    make all
    make dist
    mv alltray-*.tar.gz ..
else
    printf "Execute this script as 'utils/build-release.sh' from the main directory.\n"
    exit 1
fi
