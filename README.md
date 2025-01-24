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

For the first first version of Flamingo, to be integrated into Bob the Builder (for simple programs, like Umber and IAR), this is what I wanna get done:

- [x] `str.endswith` &co (will I have to revamp how classes work to do this?).
- [x] Sort out function scope weirdness by making all functions closures.
- [x] Vector literals (i.e. `[1, 2, 3]`).
- [x] All the other vector operations.
- [x] Unary expression.
- [x] Vector indexing getters (i.e. `v[0]`).
- [x] Anonymous functions (lambda).
- [x] Vector `.map` function.
- [x] Vector `.where` function.
- [x] (Hash)maps.
- [x] Check for existing key in map literals.
- [x] Indexing setters.
- [x] Static functions on classes (`static` qualifier) (should this be done with some kind of global singleton instance which always exists?).
- [x] External classes (I guess that's just gonna be a bunch of wrapper functions, I need to start working on Bob I think to figure out the requirements for this).

For the first complete version of Flamingo (which I'll probably need for the full AQUA build system):

- [ ] Vector slicing (i.e. `v[1:3]`).
- [ ] Vector slicing setters.
- [x] `if`/`else` and `elif` ([argument](https://langdev.stackexchange.com/questions/9/why-do-some-pl-choose-to-have-a-dedicated-keyword-for-elseif-instead-of-like-in) for why to have a dedicated `elif` keyword instead of `else if`).
- [ ] `for` loops, but only on iterators (ranges, maps, and vectors, first copied to ensure non-Turing-completeness). No `while` loops, I don't want Turing completeness!
- [ ] `break` and `continue` in loops.
- [ ] Ensure language is not Turing complete in other places (i.e. prevent recursion in anonymous functions).
- [ ] Make sure everything actually winds up getting freed (and figure out scope and value reference counting correctly).
- [ ] Ensure that strings are null-terminated in the value so that it's less annoying to integrate Flamingo with C code.

Next, I want to work on making the language enjoyable to use, and this involves hints to the LSP:

- [ ] `pure` qualifier and pure function checking (pure checking is just are we assigning variable or calling impure function in a pure one, relatively easy).
- [ ] Type checking.
- [ ] The LSP itself!
- [ ] Comprehensive docs.
- [ ] Normalize usage of type vs kind terms.
- [ ] For parser-related checks, do asserts instead of all the if checks, because these should never happen regardless of user input anyway (this also first requires checking for tree consistency, i.e. no errors in the tree).

Nice-to-haves in future versions but not super necessary for right now in my eyes:

- [ ] Operator assignment.
- [ ] String interpolation.
- [ ] Bitwise operators.
- [ ] Operator overloading. (Including for primitives? Don't know if I really want this anyway now cuz I've decided primitives will use default operators anyway. Super low priority in any case.)
