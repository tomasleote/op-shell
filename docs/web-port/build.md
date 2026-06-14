# Build web (WebAssembly via Emscripten)

Como instalar o Emscripten, compilar a op-shell para WASM e servir os artefactos
localmente. As flags e a ABI sao ditadas por `contract.md` (seccoes 6 e 7); este
documento descreve apenas o processo.

## 1. Instalar o emsdk

```sh
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh   # poe 'emcc' no PATH (so na sessao atual do shell)
```

Confirma com `emcc --version`. Em Windows usa o Git Bash ou WSL: o `build.sh` e um
script POSIX. No Windows nativo corre `emsdk_env.bat` em vez do `source`.

O `source ./emsdk_env.sh` so afeta a sessao atual. Em cada terminal novo volta a
correr esse comando antes do build.

## 2. Compilar

A partir da raiz do repo:

```sh
./web/build.sh
```

O script:
- aborta com erro claro se `emcc` nao estiver no PATH;
- aborta se `web/fs/` (conteudo MEMFS) nao existir;
- cria `web/public/` se necessario;
- compila os fontes C partilhados + `builtins_web.c` + `builtins_fs.c` e escreve os artefactos.

Saida em `web/public/`:
- `shell.js`   - glue do modulo (carrega o WASM, expoe `window.createShellModule`);
- `shell.wasm` - o binario compilado;
- `shell.data` - o MEMFS pre-carregado (`web/fs/` mapeado para `/home/guest`).

Estes tres ficheiros sao gerados; nao os edites a mao.

### Build de diagnostico

```sh
./web/build.sh --debug
```

Mesma ABI (a forma do modulo nao muda, o frontend funciona na mesma), mas troca
`-O2` por `-O0 -g3` e ativa verificacoes de runtime: `-sASSERTIONS=2`,
`-sSAFE_HEAP=1` (acessos fora dos limites), `-sSTACK_OVERFLOW_CHECK=2` e
`-fsanitize=undefined`. Em WASM um acesso invalido em `-O2` corrompe a memoria em
silencio; este modo fa-lo falhar com mensagem na consola do browser. Util ao
integrar `builtins_web.c` e ao depurar o scanner/parser. NAO usar em producao
(binario maior e mais lento).

## 3. Sem COOP/COEP

O build nao usa pthreads (sem `-pthread`, sem `SharedArrayBuffer`), por isso NAO sao
precisos os headers `Cross-Origin-Opener-Policy` / `Cross-Origin-Embedder-Policy`.
Qualquer servidor de ficheiros estatico simples chega.

## 4. Servir os .wasm/.data localmente

Os `.wasm` e `.data` tem de ser servidos por HTTP (abrir o HTML via `file://` nao
funciona). O `.wasm` deve chegar com o MIME `application/wasm`; servidores modernos
ja o fazem automaticamente.

Opcoes rapidas para testar os artefactos isolados:

```sh
# Python 3
cd web/public && python -m http.server 8000

# Node
npx serve web/public
```

Na integracao real (Seccao 5) os artefactos sao servidos pelo dev server do Vite
(`web/`); o `shell.js` e consumido via `createShellModule()` conforme a seccao 6 do
contrato.
