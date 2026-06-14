import { useEffect, useState } from 'react';
import { BOOT_LINES, BOOT_STEP_MS, BOOT_HOLD_MS } from '../constants/boot';
import { usePrefersReducedMotion } from '../hooks/usePrefersReducedMotion';

interface Props {
  ready: boolean;
  onComplete: () => void;
}

/** Retro power-on sequence. Reveals lines, then hands off once the shell is ready. */
export function BootScreen({ ready, onComplete }: Props) {
  const reduced = usePrefersReducedMotion();
  const [count, setCount] = useState(reduced ? BOOT_LINES.length : 0);

  useEffect(() => {
    if (reduced || count >= BOOT_LINES.length) return;
    const id = window.setTimeout(() => setCount((c) => c + 1), BOOT_STEP_MS);
    return () => window.clearTimeout(id);
  }, [count, reduced]);

  const linesDone = count >= BOOT_LINES.length;

  useEffect(() => {
    if (!linesDone || !ready) return;
    const id = window.setTimeout(onComplete, reduced ? 0 : BOOT_HOLD_MS);
    return () => window.clearTimeout(id);
  }, [linesDone, ready, reduced, onComplete]);

  const skip = (): void => setCount(BOOT_LINES.length);

  return (
    <div className="boot" onClick={skip}>
      <div className="boot__screen">
        <pre className="boot__lines" role="status" aria-live="polite" aria-label="Boot sequence">
          {BOOT_LINES.slice(0, count).join('\n')}
        </pre>
        {linesDone && !ready && <span className="boot__wait"> loading module...</span>}
        {(!linesDone || !ready) && <span className="boot__cursor" aria-hidden="true" />}
      </div>
    </div>
  );
}
