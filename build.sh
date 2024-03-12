#!/bin/sh
set -e

mkdir -p bin

cc_flags="-std=c11 -g -Wall -Wextra -Werror -Iflamingo/runtime -Wno-unused-parameter"

cc $cc_flags -c flamingo/flamingo.c -o bin/flamingo.o
cc $cc_flags -c main.c -o bin/main.o

cc $(find bin -name "*.o") $cc_flags -o bin/flamingo

bin/flamingo hello_world.fl

# TODO run more "serious" tests too
