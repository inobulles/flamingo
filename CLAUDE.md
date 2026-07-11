# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Commands

```bash
sh build.sh        # build → bin/flamingo
sh tests.sh        # run all tests
clang-format -i flamingo/**/*.h flamingo/**/*.c main.c  # format code
```

Run single test: `bin/flamingo tests/<name>.fl`

Update Tree-sitter grammar/runtime: `sh scripts/update-ts-flamingo.sh` / `sh scripts/update-ts-runtime.sh`

## Architecture

Flamingo is a scripting language interpreter written in C11. It uses Tree-sitter for parsing (grammar lives in external `tree-sitter-flamingo` repo; `flamingo/parser.c` is the generated output — don't edit it).

**Execution flow:** source file → Tree-sitter parse → AST traversal in `flamingo/flamingo.c` → grammar handler functions in `flamingo/grammar/*.h`

**Key components:**

- `flamingo/flamingo.h/.c` — public API and core interpreter loop; `flamingo_t` is the main interpreter struct
- `flamingo/val.h` — reference-counted value system; all values are `flamingo_val_t*`
- `flamingo/env.h` + `flamingo/scope.h` — lexical scope stack; scopes pushed/popped per block
- `flamingo/grammar/*.h` — one header per grammar construct (statement, expr, for_loop, etc.); implementations are inline in headers
- `flamingo/ptm/` — primitive type member implementations (e.g. `str.len()`, `vec.push()`)

**Value kinds** (`flamingo_val_kind_t`): `NONE`, `BOOL`, `INT`, `STR`, `VEC`, `MAP`, `FN`, `INST`

**Function kinds** (`flamingo_fn_kind_t`): `FUNCTION` (user-defined), `CLASS`, `EXTERN` (C callback), `PTM` (primitive type member)

**C integration** — external code hooks via callbacks registered on `flamingo_t`:
- `flamingo_external_fn_cb_t` — handle calls to extern functions
- `flamingo_class_decl_cb_t` / `flamingo_class_inst_cb_t` — intercept class declaration/instantiation
- `flamingo_ptm_cb_t` — implement primitive type members

## Code Conventions

- All grammar modules are `.h` files with inline implementations (not `.c`); this is intentional
- Tree-sitter nodes (`TSNode`) are passed by value throughout, following Tree-sitter convention
- Memory leak detection is currently disabled (`ASAN_OPTIONS=detect_leaks=0` in tests.sh); ASAN + UBSan are enabled at build time
- WebKit-based clang-format style; CI enforces formatting — run clang-format before committing
- Language is intentionally non-Turing-complete: no recursion, no while loops (only for-loops)
