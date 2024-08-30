# flamingo

Scripting language for AQUA 🦩

## Building

Run:

```console
sh build.sh
```

## Update the grammar

Flamingo uses Tree-sitter to parse source code. This is all defined in the [`tree-sitter-flamingo`](https://github.com/inobulles/tree-sitter-flamingo) repo. The readme there contains instructions on how to generate the parser from the grammar.

Once the parser is generated, you can just copy over the `src/parser.c` and `tree_sitter/parser.h` files to `src` in this one.

## Issues

Just so I don't have to create a million GitHub issues for all the small things that are borked:

- **Grammar**: A comment in a function is parsed as an error for some reason (actually I think I know why but changing grammar is annoying so -> later).
