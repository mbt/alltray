#!/bin/bash
#
# Generate the build system for AllTray.
#

printf "I will run ./configure after generating it and pass it all options\n"
printf "you pass to me.  Send me a SIGINT and re-run me if you want to pass\n"
printf "any configure options.\n\n"

autoreconf --install
./configure "$@"
