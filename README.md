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
