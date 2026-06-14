import { useCallback, useEffect, useState } from 'react';
import { loadShellModule } from '../shell/loadModule';
import type { ExecuteLine } from '../shell/types';

interface UseShell {
  ready: boolean;
  error: string | null;
  runLine: ExecuteLine;
}

/** Carrega o modulo (real ou mock) e expoe runLine(line) -> string. */
export function useShell(): UseShell {
  const [exec, setExec] = useState<ExecuteLine | null>(null);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    let active = true;
    loadShellModule()
      .then((mod) => {
        if (!active) return;
        const fn = mod.cwrap('execute_line', 'string', ['string']);
        setExec(() => fn);
      })
      .catch((cause: unknown) => {
        if (active) setError(cause instanceof Error ? cause.message : String(cause));
      });
    return () => {
      active = false;
    };
  }, []);

  const runLine = useCallback<ExecuteLine>((line) => (exec ? exec(line) : ''), [exec]);

  return { ready: exec !== null, error, runLine };
}
