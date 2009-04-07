#!/bin/sh

if [ "$1" = "-y" ]; then
    force=--force
else
    force=""
fi

bzr clean-tree --ignored --unknown $force

