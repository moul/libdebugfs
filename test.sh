#!/bin/sh

set -x

make re
echo
LD_PRELOAD=$(pwd)/libdebugfs.so ./test
echo