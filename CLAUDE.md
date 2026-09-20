# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

A collection of standalone CS50-style C practice exercises (Harvard CS50 2024). Each `.c` file at the
repo root is an independent, self-contained mini-program (no shared headers between exercises other than
the local `cs50.h`/`cs50.c` library). There is no single "app" — this is a learning sandbox, so files are
often incomplete, mid-edit, or intentionally buggy (e.g. `buggy.c`).

## The cs50 library

`cs50.h` / `cs50.c` is a local reimplementation of Harvard's `libcs50`, providing `get_string`,
`get_int`, `get_char`, `get_float`, `get_double`, `get_long`, `get_long_long`, and the `string` typedef
(`char *`). Every exercise includes `"cs50.h"` and must be linked against `cs50.c` — there is no
system-installed `libcs50` here. When editing an exercise, do not touch `cs50.h`/`cs50.c` unless the task
is specifically about the library itself.

## Build

Build system is CMake, with one `add_executable` target per exercise (each compiled together with
`cs50.c`):

```bash
cmake -S . -B build
cmake --build build
```

Individual targets: `hello_program`, `compare_program`, `agree_program`, `meow_program`,
`calculator_program`, `mario_program`, `buggy_program`, `scores_program`, `length_program`,
`uppercase_program`, `status_program`, `search_program`, `phonebook_program`, `stock_program`.

Build a single target:
```bash
cmake --build build --target hello_program
```

Or compile one exercise directly with no CMake involved:
```bash
gcc -std=c11 hello.c cs50.c -o hello_program
```

`stock_program` (`stock.c`) shells out to the system `curl` binary at runtime (via `popen`) to fetch quotes
from Yahoo Finance's public chart endpoint — it needs `curl` on `PATH` and network access, but no extra
build-time dependency. It also needs `_POSIX_C_SOURCE 200809L` defined before `popen`/`pclose` are visible
under `-std=c11`.

## Adding a new exercise

1. Create `<name>.c` at the repo root, `#include "cs50.h"` as needed.
2. Add a corresponding line to `CMakeLists.txt`: `add_executable(<name>_program <name>.c cs50.c)`.

## Notes on individual files

- `buggy.c` is intentionally left with a bug for debugging practice — don't "fix" it unless asked.
- `length.c` has a commented-out manual reimplementation of `strlen` (`string_length`) below `main`; the
  active code just calls `strlen` directly.
- `phonebook.c` is incomplete (parallel `names`/`numbers` arrays declared, no lookup logic yet).
- `status.c` declares `main(int argc, string argv)` (should be `string argv[]`); it compiles with a
  `-Wformat` warning and `argv[1]` is technically indexing a scalar, not an array — don't treat the warning
  as noise from an unrelated change.
