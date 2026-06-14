/** Retro boot sequence, revealed line by line before the terminal appears. */
export const BOOT_LINES = [
  'op-shell v0.1  -  WebAssembly build',
  '',
  '[ ok ]  power-on self test',
  '[ ok ]  mounting memfs at /home/guest',
  '[ ok ]  loading webassembly module',
  '[ ok ]  initializing scanner + parser',
  '[ ok ]  starting interactive shell',
];

export const BOOT_STEP_MS = 240;
export const BOOT_HOLD_MS = 520;
