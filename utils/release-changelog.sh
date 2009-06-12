#!/bin/sh
#
# release-changelog.sh
#
# Update the ChangeLog file in the root of the repository to include new
# changes, generated from bzr.
#

TMPFILE=/tmp/alltray-changes.$$
STARTREV="$1"

if [ ! -w ChangeLog ]; then
    printf "ChangeLog does not exist or exists but is not writeable.\n"
    printf "This script must be run from the root directory of the AllTray checkout.\n"
    exit 1
fi

bzr log --gnu -v -r${STARTREV}..-1 > ${TMPFILE}
cat ChangeLog >> ${TMPFILE}
mv ${TMPFILE} ChangeLog

printf "ChangeLog updated.\n"
