#!/bin/bash

if [ -x ./autogen.sh ]; then
    utils/clean-tree.sh
    ./autogen.sh CFLAGS="-Wall -ggdb -Os"

    make all

    objcopy --only-keep-debug src/alltray src/alltray.dbg
    objcopy --strip-debug src/alltray
    objcopy --add-gnu-debuglink=src/alltray.dbg src/alltray

    make dist
    mv alltray-*.tar.gz ..
else
    printf "Execute this script as 'utils/build-release.sh' from the main directory.\n"
    exit 1
fi
