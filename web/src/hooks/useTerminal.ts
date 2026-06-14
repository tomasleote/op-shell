import { useCallback, useRef, useState } from 'react';
import type { ExecuteLine } from '../shell/types';
import type { TerminalBlock } from '../types';
import { BOOT_BANNER, PROMPT } from '../constants/banner';

const FORM_FEED = '\f';

/** Estado dos blocos do terminal + protocolo CLEAR ('\f' limpa o ecra). */
export function useTerminal(runLine: ExecuteLine) {
  const [blocks, setBlocks] = useState<TerminalBlock[]>([
    { id: 0, kind: 'banner', text: BOOT_BANNER },
  ]);
  const idRef = useRef(1);
  const nextId = (): number => idRef.current++;

  const submit = useCallback(
    (line: string): void => {
      const out = runLine(line);
      const echo: TerminalBlock = { id: nextId(), kind: 'input', text: PROMPT + line };

      if (out.includes(FORM_FEED)) {
        const tail = out.slice(out.lastIndexOf(FORM_FEED) + 1);
        setBlocks(tail.length > 0 ? [{ id: nextId(), kind: 'output', text: tail }] : []);
        return;
      }

      const appended: TerminalBlock[] =
        out.length > 0 ? [echo, { id: nextId(), kind: 'output', text: out }] : [echo];
      setBlocks((prev) => [...prev, ...appended]);
    },
    [runLine],
  );

  return { blocks, submit };
}
