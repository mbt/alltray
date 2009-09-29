#! /bin/sh
#
# Generate the build system for AllTray.
#

if [ "$1" == "--no-configure" ]; then
    autoreconf --force --install
else
    printf "I will run ./configure after generating it and pass it all options\n"
    printf "you pass to me.  Send me a SIGINT and re-run me if you want to pass\n"
    printf "any configure options.\n\n"

    autoreconf --force --install
    ./configure "$@"
fi
