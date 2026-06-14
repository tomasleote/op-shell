/**
 * Intro shown once on first entry, before the first prompt.
 * Short by design; the long story lives in readme.txt (Section 3).
 */
export const BOOT_BANNER = [
  'op-shell  //  Operating Systems course project',
  '',
  '  A small Unix-like shell written in C and compiled to WebAssembly,',
  '  so it runs right here in your browser. The very same source code also',
  "  builds as a native binary: clone the repo and run 'make && ./shell'.",
  '',
  '  Why: to prove that one real C codebase - the actual scanner, parser',
  '  and command logic - can target both a POSIX machine and the web,',
  '  with the build target chosen at compile time and no rewrite.',
  '',
  '  Getting started:',
  '    help              list the available commands',
  '    cat readme.txt    read the full story',
  '    ls                see what lives in your home folder',
].join('\n');

export const PROMPT = 'guest@op-shell:~$ ';
