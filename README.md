# op-shell

A command-line shell written in C for an Operating Systems course. From a single
codebase it runs in two places:

1. **Native (Linux/macOS)** - a real shell that launches real programs with
   `fork()` + `execvp()`.
2. **Browser** - the same C compiled to WebAssembly with Emscripten, running over
   an in-memory filesystem, with no install and no backend.

The scanner, parser and command model are shared and identical in both targets.
Only command execution diverges, selected at compile time with
`#ifdef __EMSCRIPTEN__`. Nothing about the original native shell was removed to
make the web version work.

## Run it natively

```sh
make
./shell
```

This builds with `gcc` and behaves like a normal shell: it resolves commands in
`$PATH` and runs them as child processes (`fork()` + `execvp()`).

## Run it in the browser (WebAssembly)

Prerequisites: the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)
(`emcc` on `PATH`) and Node.js. Emscripten setup details are in
[docs/web-port/build.md](docs/web-port/build.md).

```sh
# 1. Compile the C to WebAssembly (produces web/public/shell.mjs + .wasm + .data)
./web/build.sh

# 2. Run the web app
cd web
npm install
VITE_USE_REAL_SHELL=true npm run dev      # the real WASM shell
```

`npm run dev` on its own uses an in-memory JavaScript mock with the same module
interface, for fast UI iteration without rebuilding the WASM. Set
`VITE_USE_REAL_SHELL=true` (dev or `npm run build`) to load the real `shell.mjs`.

## How it fits together

| File | Role | Target |
|------|------|--------|
| `scanner.c` / `.h` | tokenizer | shared |
| `parser.c` / `.h` | recursive-descent parser to a `Command` list | shared |
| `command.c` / `.h` | the `Command` model | shared |
| `shell.c` | execution: `fork()`+`execvp()` natively; dispatch to web built-ins under Emscripten | shared |
| `main.c` | native REPL; under Emscripten only sets the home directory | shared |
| `builtins_web.c` / `builtins_fs.c` | browser built-ins over MEMFS: `execute_line()` plus the command set below | web only |
| `web/` | Vite + React terminal UI, the `emcc` build script, and the virtual filesystem | web only |

In the browser there are no external programs to `exec`, so commands a native
shell would launch as binaries (`ls`, `cat`, `cd`, ...) are implemented as C
built-ins that operate on Emscripten's MEMFS. The real scanner and parser still
tokenize and parse every line - run `parse <line>` to see the tokens and the
parsed command structure produced by the actual course code.

### Web command set

`help`, `ls [dir]`, `cd [dir]`, `pwd`, `cat <file>`, `echo [text]`,
`mkdir <dir>`, `touch <file>`, `rm [-r] <path>`, `clear`, `status`, `exit`,
`parse <line>`.

## Documentation

- [docs/web-port/contract.md](docs/web-port/contract.md) - module interfaces and file ownership across the port.
- [docs/web-port/build.md](docs/web-port/build.md) - Emscripten installation and build details.
