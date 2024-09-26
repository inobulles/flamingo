# flamingo

Scripting language for AQUA 🦩

## Building

Run:

```console
sh build.sh
```

Then, to run the tests:

```console
sh test.sh
```

## Update the grammar

Flamingo uses Tree-sitter to parse source code. This is all defined in the [`tree-sitter-flamingo`](https://github.com/inobulles/tree-sitter-flamingo) repo. The readme there contains instructions on how to generate the parser from the grammar.

Once the parser is generated, you can just copy over the `src/parser.c` and `tree_sitter/parser.h` files to `src` in this one.

Alternatively, there are two functions in `scripts/` which you can run to update the Tree-sitter runtime and the grammar:

```console
sh scripts/update-ts-runtime.sh # Update the Tree-sitter runtime.
sh scripts/update-ts-flamingo.sh [optional repo URL, can be file://] # Update the Flamingo grammar.
```

## Roadmap

For the first version of Flamingo, to be integrated into Bob the Builder, this is what I wanna get done:

- [ ] `str.endswith` &co (will I have to revamp how classes work to do this?).
- [ ] Static functions on classes (`static` qualifier).
- [ ] `if`/`else` and `elif` ([argument](https://langdev.stackexchange.com/questions/9/why-do-some-pl-choose-to-have-a-dedicated-keyword-for-elseif-instead-of-like-in) for why to have a dedicated `elif` keyword instead of `else if`).
- [ ] Vector literals (i.e. `[1, 2, 3]`).
- [ ] Vector getters and setters (i.e. `v[0]`).
- [ ] Vector slicing (i.e. `v[1:3]`).
- [ ] All the other vector operations.
- [ ] `for` loops, but only on iterators (ranges, maps, and vectors, first copied to ensure non-Turing-completeness). No `while` loops, I don't want Turing completeness!
- [ ] `break` and `continue` in loops.
- [ ] Ensure language is not Turing complete in other places (i.e. no recursion).
- [ ] Anonymous functions.

Next, I want to work on making the language enjoyable to use, and this involves hints to the LSP:

- [ ] `pure` qualifier and pure function checking (pure checking is just are we assigning variable or calling impure function in a pure one, relatively easy).
- [ ] Type checking.
- [ ] Sort out function scope weirdness.
- [ ] The LSP itself!
- [ ] Comprehensive docs.

Nice-to-haves in future versions but not super necessary for right now in my eyes:

- [ ] Operator assignment.
- [ ] String interpolation.
- [ ] Bitwise operators.
- [ ] Operator overloading. (Including for primitives? Don't know if I really want this anyway now cuz I've decided primitives will use default operators anyway. Super low priority in any case.)
