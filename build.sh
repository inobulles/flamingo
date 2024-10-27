#!/bin/sh
set -e

if [ -z "$CC" ]; then
	CC=cc
fi

mkdir -p bin

cc_flags="-std=c11 -g -Wall -Wextra -Werror -Iflamingo/runtime -Wno-unused-parameter"

# XXX With the default error limit, clangd tells us that there are too many errors and it's stopping here.
#     When the error limit is disabled like I'm doing here, it says there are no errors.
#     Could this be a clangd bug?

$CC $cc_flags -D_DEFAULT_SOURCE -D_GNU_SOURCE -ferror-limit=0 -c flamingo/flamingo.c -o bin/flamingo.o
$CC $cc_flags -D_DEFAULT_SOURCE -c main.c -o bin/main.o

$CC $(find bin -name "*.o") -lm $cc_flags -o bin/flamingo
