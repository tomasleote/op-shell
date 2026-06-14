import type { ShellModule } from './types';
import readme from '../../fs/readme.txt?raw';
import aboutMe from '../../fs/about-me.txt?raw';
import contacts from '../../fs/contacts.txt?raw';
import opShell from '../../fs/projects/op-shell.txt?raw';
import secret from '../../fs/.secret.txt?raw';
import vault from '../../fs/.secrets/vault.txt?raw';

/**
 * Dev mock: same shape as the contract interface (section 6), with a tiny
 * in-memory shell so the UI can be exercised before the real C->WASM build
 * (sections 2 and 4) exists. The file contents are the real Section 3 files
 * (web/fs), imported as raw strings so there is no duplicated copy to drift.
 * Replaced by the real '/shell.js' when VITE_USE_REAL_SHELL=true.
 */

const ROOT = '/home/guest';

const FILES: Readonly<Record<string, string>> = {
  '/home/guest/readme.txt': readme,
  '/home/guest/about-me.txt': aboutMe,
  '/home/guest/contacts.txt': contacts,
  '/home/guest/projects/op-shell.txt': opShell,
  '/home/guest/.secret.txt': secret,
  '/home/guest/.secrets/vault.txt': vault,
};

const DIRS: ReadonlySet<string> = new Set([
  '/home/guest',
  '/home/guest/projects',
  '/home/guest/.secrets',
]);

const HELP = [
  'available commands:',
  '  help             this help',
  '  ls [dir]         list a directory',
  '  cd [dir]         change directory (cd .. goes up)',
  '  pwd              print the working directory',
  '  cat <file>       print a file',
  '  echo [text]      repeat text',
  '  clear            clear the screen',
  '  status           exit code of the last command',
  '  exit             farewell (does not kill the runtime)',
].join('\n');

let cwd = ROOT;

function normalize(path: string): string {
  const out: string[] = [];
  for (const part of path.split('/')) {
    if (part === '' || part === '.') continue;
    if (part === '..') out.pop();
    else out.push(part);
  }
  return '/' + out.join('/');
}

function resolve(arg: string): string {
  return normalize(arg.startsWith('/') ? arg : cwd + '/' + arg);
}

function baseName(path: string): string {
  return path.slice(path.lastIndexOf('/') + 1);
}

function listDir(dir: string): string {
  const prefix = dir + '/';
  const entries = new Set<string>();
  for (const path of [...Object.keys(FILES), ...DIRS]) {
    if (!path.startsWith(prefix)) continue;
    const name = path.slice(prefix.length).split('/')[0];
    if (!name.startsWith('.')) entries.add(name); // hidden entries stay hidden
  }
  return [...entries].sort().join('  ') + '\n';
}

function ls(arg?: string): string {
  const target = arg ? resolve(arg) : cwd;
  if (DIRS.has(target)) return listDir(target);
  if (FILES[target] !== undefined) return baseName(target) + '\n';
  return `ls: ${arg}: no such file or directory\n`;
}

function cd(arg?: string): string {
  const target = arg ? resolve(arg) : ROOT;
  if (DIRS.has(target)) {
    cwd = target;
    return '';
  }
  if (FILES[target] !== undefined) return `cd: ${arg}: not a directory\n`;
  return `cd: ${arg}: no such file or directory\n`;
}

function cat(arg?: string): string {
  if (arg === undefined) return 'cat: missing operand\n';
  const target = resolve(arg);
  if (FILES[target] !== undefined) return FILES[target] + '\n';
  if (DIRS.has(target)) return `cat: ${arg}: is a directory\n`;
  return `cat: ${arg}: no such file or directory\n`;
}

function execute(line: string): string {
  const trimmed = line.trim();
  if (trimmed.length === 0) return '';

  const [cmd, ...args] = trimmed.split(/\s+/);

  switch (cmd) {
    case 'help':
      return HELP + '\n';
    case 'clear':
      return '\f';
    case 'pwd':
      return cwd + '\n';
    case 'echo':
      return args.join(' ') + '\n';
    case 'ls':
      return ls(args[0]);
    case 'cd':
      return cd(args[0]);
    case 'cat':
      return cat(args[0]);
    case 'status':
      return '0\n';
    case 'exit':
      return 'Session ended (mock). Reload the page to start over.\n';
    default:
      return `${cmd}: command not found (try 'help')\n`;
  }
}

export function createMockShellModule(): Promise<ShellModule> {
  cwd = ROOT;
  return Promise.resolve({
    cwrap: () => execute,
  });
}
