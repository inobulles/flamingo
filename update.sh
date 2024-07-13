#!/bin/sh
set -xe

# Update Tree-sitter runtime.

rm -rf tree-sitter 2>/dev/null || true
rm -r src/runtime 2>/dev/null || true

git clone https://github.com/tree-sitter/tree-sitter --depth 1 --branch master
mkdir -p src/runtime

mv tree-sitter/lib/include/tree_sitter src/runtime/
mv tree-sitter/lib/src/* src/runtime

rm -rf tree-sitter

# Update tree-sitter-flamingo (i.e. src/parser.c and src/tree_sitter/parser.h).

rm -rf tree-sitter-flamingo 2>/dev/null || true
git clone https://github.com/inobulles/tree-sitter-flamingo --depth 1 --branch main

mv tree-sitter-flamingo/src/parser.c flamingo/parser.c
mv tree-sitter-flamingo/src/tree_sitter/parser.h flamingo/tree_sitter/parser.h

rm -rf tree-sitter-flamingo
