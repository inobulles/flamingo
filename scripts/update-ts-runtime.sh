#!/bin/sh
set -xe

# Update Tree-sitter runtime.

rm -rf tree-sitter 2>/dev/null || true
rm -r flamingo/runtime 2>/dev/null || true

git clone https://github.com/tree-sitter/tree-sitter --depth 1 --branch master
mkdir -p flamingo/runtime

mv tree-sitter/lib/include/tree_sitter flamingo/runtime/
mv tree-sitter/lib/src/* flamingo/runtime

rm -rf tree-sitter
