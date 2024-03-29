# !/bin/bash

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd $srcdir

autoreconf --force -v --install || exit 1
cd $ORIGDIR || exit $?

$srcdir/configure "$@"
