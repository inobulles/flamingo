#!/bin/sh
set -e

rm -r bin 2>/dev/null || true
mkdir -p bin

cc -c -o bin/flamingo.o flamingo.c
cc -c -o bin/main.o main.c

cc -o bin/flamingo bin/flamingo.o bin/main.o
bin/flamingo hello_world.fl

# TODO run more "serious" tests too
