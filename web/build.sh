#!/usr/bin/env bash
# Compila a op-shell para WebAssembly via Emscripten.
# Fonte de verdade das flags e ABI: docs/web-port/contract.md (seccoes 6 e 7).
#
# Uso:
#   ./build.sh            build de producao (flags exatas do contrato, -O2)
#   ./build.sh --debug    build de diagnostico: ASSERTIONS, SAFE_HEAP,
#                         STACK_OVERFLOW_CHECK e UBSan. Mesma ABI, mas apanha
#                         out-of-bounds e UB no browser (nao usar em producao).
set -euo pipefail

MODE="release"
if [ "${1:-}" = "--debug" ]; then
  MODE="debug"
elif [ "$#" -gt 0 ]; then
  echo "erro: argumento desconhecido '$1' (usa --debug ou nenhum)." >&2
  exit 1
fi

# Corre sempre a partir da pasta do script para que os caminhos relativos
# (../*.c, fs, public) sejam estaveis independentemente do cwd de invocacao.
cd "$(dirname "$0")"

if ! command -v emcc >/dev/null 2>&1; then
  echo "erro: 'emcc' nao encontrado no PATH." >&2
  echo "      Instala o emsdk e corre 'source ./emsdk_env.sh' antes do build." >&2
  echo "      Detalhes em docs/web-port/build.md." >&2
  exit 1
fi

if [ ! -d fs ]; then
  echo "erro: pasta 'web/fs' nao existe (conteudo MEMFS da Seccao 3)." >&2
  echo "      E pre-carregada para /home/guest via --preload-file." >&2
  exit 1
fi

mkdir -p public

SOURCES=(
  ../scanner.c
  ../parser.c
  ../command.c
  ../shell.c
  ../main.c
  ../builtins_web.c
  ../builtins_fs.c
)

# MODULARIZE expoe a factory createShellModule. NAO usamos EXPORT_ES6: o
# frontend carrega shell.js via <script> tag (o Vite proibe import() de
# ficheiros em /public), e a factory fica em window.createShellModule.
ABI_FLAGS=(
  -sMODULARIZE=1 -sEXPORT_NAME=createShellModule
  -sEXPORTED_FUNCTIONS=_execute_line,_main
  -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS
  -sEXIT_RUNTIME=0 -sALLOW_MEMORY_GROWTH=1
  --preload-file fs@/home/guest
)

if [ "$MODE" = "debug" ]; then
  MODE_FLAGS=(
    -O0 -g3
    -sASSERTIONS=2 -sSAFE_HEAP=1 -sSTACK_OVERFLOW_CHECK=2
    -fsanitize=undefined
  )
else
  MODE_FLAGS=(-O2)
fi

echo "build: modo $MODE, a compilar ${#SOURCES[@]} fontes C para web/public/shell.js"

emcc "${SOURCES[@]}" \
  -o public/shell.js \
  "${ABI_FLAGS[@]}" \
  "${MODE_FLAGS[@]}"

echo "build: concluido. Artefactos em web/public/: shell.js shell.wasm shell.data"
