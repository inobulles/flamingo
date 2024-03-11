#!/bin/sh
set -xe

# update Tree-sitter runtime

rm -rf tree-sitter 2>/dev/null || true
rm -r src/runtime 2>/dev/null || true

git clone https://github.com/tree-sitter/tree-sitter --depth 1 --branch master
mkdir -p src/runtime

mv tree-sitter/lib/include/tree_sitter src/runtime/
mv tree-sitter/lib/src/* src/runtime

rm -rf tree-sitter

# TODO update tree-sitter-flamingo (i.e. src/parser.c and src/tree_sitter/parser.h)
