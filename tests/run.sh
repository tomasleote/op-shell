#!/usr/bin/env bash
# Builds and runs the C test suite. Zero dependencies beyond a C compiler.
# Compiles the shared front-end (scanner/parser/command) plus shell.c, so it
# must run on a fork-capable toolchain (Linux, WSL, or Cygwin). Override the
# compiler with CC=... if needed.
set -euo pipefail

cd "$(dirname "$0")"

CC="${CC:-cc}"
if ! command -v "$CC" >/dev/null 2>&1; then
  echo "erro: compilador '$CC' nao encontrado. Define CC=<compilador>." >&2
  exit 1
fi

SOURCES=(
  ../scanner.c
  ../parser.c
  ../command.c
  ../shell.c
  test_scanner.c
  test_parser.c
  test_command.c
  test_builtins.c
  test_main.c
)

echo "tests: a compilar com $CC"
# gnu11 (nao c11) expoe funcoes POSIX como strdup; sem isto, strdup fica com
# declaracao implicita e o ponteiro de 64 bits e truncado -> segfault.
"$CC" -std=gnu11 -Wall -Wextra -I.. "${SOURCES[@]}" -o run_tests

echo "tests: a correr"
./run_tests
