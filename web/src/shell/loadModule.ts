import type { ShellModule, ShellModuleFactory } from './types';
import { createMockShellModule } from './mockModule';

const REAL_SHELL_URL = '/shell.js';

declare global {
  interface Window {
    createShellModule?: ShellModuleFactory;
  }
}

/** Loads a classic script once and resolves after it has executed. */
function loadScript(src: string): Promise<void> {
  return new Promise((resolve, reject) => {
    if (window.createShellModule) {
      resolve(); // already executed (e.g. StrictMode's second mount)
      return;
    }
    const existing = document.querySelector<HTMLScriptElement>(`script[data-shell="${src}"]`);
    if (existing) {
      existing.addEventListener('load', () => resolve());
      existing.addEventListener('error', () => reject(new Error(`failed to load ${src}`)));
      return;
    }
    const script = document.createElement('script');
    script.src = src;
    script.dataset.shell = src;
    script.onload = () => resolve();
    script.onerror = () => reject(new Error(`failed to load ${src}`));
    document.head.appendChild(script);
  });
}

/**
 * Returns the real module (Section 4) when VITE_USE_REAL_SHELL=true, otherwise
 * the dev mock. The real shell.js lives in /public, which Vite refuses to let
 * source code import(), so it is loaded via a <script> tag; the MODULARIZE build
 * exposes its factory as window.createShellModule.
 */
export async function loadShellModule(): Promise<ShellModule> {
  if (import.meta.env.VITE_USE_REAL_SHELL === 'true') {
    await loadScript(REAL_SHELL_URL);
    const factory = window.createShellModule;
    if (!factory) {
      throw new Error('shell.js loaded but did not expose createShellModule');
    }
    return factory();
  }

  return createMockShellModule();
}
