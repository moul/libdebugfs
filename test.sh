#!/bin/sh

set -x

make
echo
LD_PRELOAD=$(pwd)/libdebugfs.so ./test
echo