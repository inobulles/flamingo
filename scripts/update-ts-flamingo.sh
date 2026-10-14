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
git clone $URL --depth 1 --branch v0.1.4

mv tree-sitter-flamingo/src/parser.c flamingo/parser.c
mv tree-sitter-flamingo/src/tree_sitter/parser.h flamingo/tree_sitter/parser.h

# Embed the syntax highlighting query (used for colouring error snippets) as a C string.

{
	echo '// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.'
	echo '// Copyright (c) 2024-2026 Aymeric Wibo'
	echo ''
	echo "// AUTO-GENERATED from tree-sitter-flamingo's 'queries/highlights.scm' by 'scripts/update-ts-flamingo.sh'."
	echo '// Do not edit by hand.'
	echo ''
	echo '#pragma once'
	echo ''
	echo 'static char const flamingo_highlights_query[] ='
	sed -e 's/\\/\\\\/g' -e 's/"/\\"/g' -e 's/^/\t"/' -e 's/$/\\n"/' tree-sitter-flamingo/queries/highlights.scm
	echo ';'
} > flamingo/highlights.h

rm -rf tree-sitter-flamingo
