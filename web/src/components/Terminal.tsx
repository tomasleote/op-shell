import { useEffect, useRef, useState } from 'react';
import type { TerminalBlock } from '../types';
import { TerminalOutput } from './TerminalOutput';
import { TerminalInput } from './TerminalInput';

interface Props {
  blocks: readonly TerminalBlock[];
  onSubmit: (line: string) => void;
  ready: boolean;
  error: string | null;
}

export function Terminal({ blocks, onSubmit, ready, error }: Props) {
  const inputRef = useRef<HTMLInputElement>(null);
  const bottomRef = useRef<HTMLDivElement>(null);
  const [scanlines, setScanlines] = useState(true);

  useEffect(() => {
    inputRef.current?.focus();
  }, []);

  useEffect(() => {
    bottomRef.current?.scrollIntoView({ block: 'end' });
  }, [blocks]);

  const focusInput = (): void => inputRef.current?.focus();

  return (
    <div className={`crt${scanlines ? ' crt--scanlines' : ''}`}>
      <div className="crt__bezel" onClick={focusInput}>
        <label className="crt__toggle" onClick={(event) => event.stopPropagation()}>
          <input
            type="checkbox"
            checked={scanlines}
            onChange={(event) => setScanlines(event.target.checked)}
          />
          scanlines
        </label>

        <div className="crt__scroll">
          <div className="crt__log" role="log" aria-live="polite" aria-label="Saida do terminal">
            <TerminalOutput blocks={blocks} />
            {error !== null && <pre className="block block--error">erro: {error}</pre>}
          </div>
          <TerminalInput ref={inputRef} onSubmit={onSubmit} disabled={!ready} />
          <div ref={bottomRef} />
        </div>

        <div className="crt__scanline-overlay" aria-hidden="true" />
      </div>
    </div>
  );
}
