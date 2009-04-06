#!/bin/bash

if [ -x ./autogen.sh ]; then
    ./autogen.sh CFLAGS="-Wall -Wextra -ggdb -O0"
    make
else
    printf "Execute this script as 'utils/build-full-debug.sh' from the main directory.\n"
    exit 1
fi
