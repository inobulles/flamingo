#!/bin/sh
set -xe

URL=https://github.com/inobulles/tree-sitter-flamingo

if [ $# -gt 1 ]; then
	echo "Usage: scripts/update-ts-flamingo.sh [tree-sitter-flamingo URL]"
	exit 1
fi

if [ $# -eq 1 ]; then
	URL=$1
fi

# Update tree-sitter-flamingo (i.e. src/parser.c and src/tree_sitter/parser.h).

rm -rf tree-sitter-flamingo 2>/dev/null || true
git clone $URL --depth 1 --branch main

mv tree-sitter-flamingo/src/parser.c flamingo/parser.c
mv tree-sitter-flamingo/src/tree_sitter/parser.h flamingo/tree_sitter/parser.h

rm -rf tree-sitter-flamingo
