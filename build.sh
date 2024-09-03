#!/bin/sh
set -e

mkdir -p bin

cc_flags="-std=c11 -g -Wall -Wextra -Werror -Iflamingo/runtime -Iflamingo -Wno-unused-parameter"

# XXX With the default error limit, clangd tells us that there are too many errors and it's stopping here.
#     When the error limit is disabled like I'm doing here, it says there are no errors.
#     Could this be a clangd bug?

cc $cc_flags -ferror-limit=0 -c flamingo/flamingo.c -o bin/flamingo.o
cc $cc_flags -c main.c -o bin/main.o

cc $(find bin -name "*.o") -lm $cc_flags -o bin/flamingo

bin/flamingo hello_world.fl

# TODO Run more "serious" tests too.
