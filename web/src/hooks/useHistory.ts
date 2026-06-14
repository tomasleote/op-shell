import { useRef, useState } from 'react';

interface UseHistory {
  add: (line: string) => void;
  previous: () => string | null;
  next: () => string | null;
}

/**
 * Historico de comandos com navegacao por setas.
 * cursor === -1 significa "linha nova" (a editar, ainda nao no historico).
 */
export function useHistory(): UseHistory {
  const [entries, setEntries] = useState<string[]>([]);
  const cursor = useRef(-1);

  const add = (line: string): void => {
    cursor.current = -1;
    if (line.trim().length === 0) return;
    setEntries((prev) => [...prev, line]);
  };

  const previous = (): string | null => {
    if (entries.length === 0) return null;
    cursor.current = cursor.current === -1 ? entries.length - 1 : Math.max(0, cursor.current - 1);
    return entries[cursor.current];
  };

  const next = (): string | null => {
    if (cursor.current === -1) return null;
    if (cursor.current >= entries.length - 1) {
      cursor.current = -1;
      return '';
    }
    cursor.current += 1;
    return entries[cursor.current];
  };

  return { add, previous, next };
}
