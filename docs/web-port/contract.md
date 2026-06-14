# Web Port - Contrato Partilhado (fonte de verdade)

Objetivo: ditar interfaces e propriedade de ficheiros para que as Seccoes 2-5 sejam
construidas EM PARALELO, em chats separados, SEM tocarem nos mesmos ficheiros.
Le isto por inteiro antes de comecar a tua fatia. O design ja esta aprovado: isto
e implementacao, nao faças brainstorming.

## 1. Principio dual-target
- scanner.c, parser.c, command.c sao PARTILHADOS e compilados nos dois alvos (o codigo real de OS).
- Build nativo (Makefile da raiz, `make && ./shell`): comportamento INALTERADO - fork()+execvp(),
  REPL bloqueante com getchar(). TEM de continuar a funcionar.
- Build web (`web/build.sh`, emcc): sem fork/exec; comandos sao built-ins em C sobre o MEMFS;
  o JS conduz o loop chamando a funcao exportada `execute_line`.
- A divergencia e escolhida em tempo de compilacao com `#ifdef __EMSCRIPTEN__`.

## 2. Propriedade de ficheiros (DISJUNTA - mexe so nos teus)
- Seccao 2 (C built-ins + port): `builtins_web.c`, `builtins_web.h` [+ opcional `builtins_fs.c`],
  e edicoes a `shell.c`, `main.c`, `scanner.c`, `parser.c`.
- Seccao 3 (conteudo MEMFS): SO `web/fs/**`.
- Seccao 4 (pipeline emcc): `web/build.sh`, `docs/web-port/build.md`. Artefactos gerados em
  `web/public/shell.js|wasm|data`.
- Seccao 5 (terminal React): app Vite em `web/` - `web/src/**`, `web/index.html`,
  `web/package.json`, `web/vite.config.ts`, `web/tsconfig.json`, `web/.gitignore`.
  NAO toca em `web/fs`, NAO toca em `web/build.sh`, NAO escreve `web/public/shell.*`.
- Orquestrador (integracao): README da raiz, wiring final, gate de integracao.

Ninguem edita o Makefile da raiz (o build nativo fica byte-identico).

## 3. ABI do C (Seccao 2 implementa; Seccao 4 exporta; Seccao 5 chama)
```c
char* execute_line(const char* line);
```
- Corre UMA linha pelo scanner+parser reais e executa via built-ins web.
- Devolve um ponteiro para um buffer detido pelo modulo, valido ate a PROXIMA chamada
  (o JS copia logo via cwrap 'string'). O JS nao liberta memoria.
- Output UTF-8, pode conter '\n'.
- Protocolo CLEAR: para limpar o ecra, o output contem o caractere form-feed '\f' (0x0C).
  A Seccao 5 deteta '\f' e limpa.
- `main()` web: sob __EMSCRIPTEN__, NAO corre o REPL; faz `chdir("/home/guest")` e retorna.
  Runtime mantem-se vivo (EXIT_RUNTIME=0).
- `exit` (web): NAO pode chamar exit()/matar o modulo; imprime uma despedida.

## 4. Conjunto de built-ins web (Seccao 2)
`help`, `ls [dir]`, `cd [dir]`, `pwd`, `cat <file>`, `echo [args...]`, `mkdir <dir>`,
`touch <file>`, `rm [-r] <path>`, `clear`, mais os existentes `exit`/`status` (web-safe).
Opcional (mostra os internals reais): `parse <line>` -> imprime a lista de tokens e/ou a
lista de Command usando o printList/printCommands existentes.
Implementa com POSIX sobre MEMFS: opendir/readdir/stat/getcwd/chdir/mkdir/unlink/rmdir/fopen.
Cada ficheiro <=200 linhas (divide em builtins_fs.c se preciso).

## 5. Layout MEMFS (conteudo da Seccao 3; pre-carregado pela Seccao 4 em /home/guest)
- cwd no boot = `/home/guest`. A pasta fonte `web/fs/` e mapeada para `/home/guest`.
- Arvore sugerida (a Seccao 3 tem liberdade nos detalhes, tudo sob `web/fs/`):
  `readme.txt`, `sobre-mim.txt`, `contactos.txt`, `projetos/*.txt`, `.secrets/...`
- NAO inventes dados pessoais do Tomas - usa placeholders marcados `[PREENCHER: ...]`.

## 6. Interface do modulo WASM (Seccao 4 produz; Seccao 5 consome)
- Build com: MODULARIZE=1, EXPORT_NAME=createShellModule, saida `web/public/shell.js`.
```js
const Module = await createShellModule();
const execLine = Module.cwrap('execute_line', 'string', ['string']);
const out = execLine(line);            // string, pode conter '\n' e '\f'
```
- Mock de dev da Seccao 5 (mesma forma): `createShellModule()` resolve para
  `{ cwrap: () => (line) => "...\n" }`. Troca mock -> '/shell.js' via flag de ambiente Vite.

## 7. Flags emcc (Seccao 4)
```
emcc ../scanner.c ../parser.c ../command.c ../shell.c ../main.c \
  ../builtins_web.c ../builtins_fs.c \
  -o public/shell.js \
  -sMODULARIZE=1 -sEXPORT_NAME=createShellModule \
  -sEXPORTED_FUNCTIONS=_execute_line,_main \
  -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS \
  -sEXIT_RUNTIME=0 -sALLOW_MEMORY_GROWTH=1 \
  --preload-file fs@/home/guest -O2
```
(Sem pthreads -> nao sao precisos headers COOP/COEP.)

## 8. Intro de boot (Seccao 5 renderiza; esta copia e a canonica)
No arranque, antes do primeiro prompt, mostrar este banner (estilizado, CRT). Curto;
a historia longa vive no readme.txt (Seccao 3):
```
op-shell  -  projeto da cadeira de Operating Systems
Shell escrita em C, compilada para WebAssembly, a correr no teu browser.
O mesmo codigo corre num terminal real: clona o repo e faz 'make && ./shell'.
Escreve 'help' para comecar, ou 'cat readme.txt'.
```
(O Tomas pode afinar o texto.)

## 9. Convencoes (TODAS as seccoes)
- Ficheiros <=200 linhas, uma responsabilidade.
- UI: todas as cores/spacing/tipografia via design tokens - nada hardcoded.
- TS: strict, sem any; named exports; const sobre let; estilo funcional; async/await.
- Sem emojis no codigo/output/logs; sem em-dashes nos docs; sem atribuicao de IA; nao criar commits git.
- Comentar so o PORQUE nao-obvio. Sem resumos finais.
- Cirurgico: so os teus ficheiros. Se precisares de mudar uma interface partilhada, PARA e reporta.
- Nao inventar dados pessoais do Tomas.
- Verificar antes de dizer "feito" (nativo: `make`; web: build; UI: build + abrir num browser real).

## 10. Integracao (orquestrador, depois dos 4 chats)
1. Build web (Seccao 4) contra o C real (Seccao 2) + fs (Seccao 3).
2. Trocar o mock da Seccao 5 pelo `/shell.js` real.
3. Gate: `make` nativo passa; build web passa; abrir no browser e smoke-test manual.
4. README da raiz a explicar os dois modos de correr.
