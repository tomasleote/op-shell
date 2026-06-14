import { useCallback, useState } from 'react';
import { useShell } from './hooks/useShell';
import { useTerminal } from './hooks/useTerminal';
import { Terminal } from './components/Terminal';
import { BootScreen } from './components/BootScreen';
import './styles/tokens.css';
import './styles/boot.css';
import './styles/terminal.css';

export function App() {
  const { ready, error, runLine } = useShell();
  const { blocks, submit } = useTerminal(runLine);
  const [booted, setBooted] = useState(false);
  const finishBoot = useCallback(() => setBooted(true), []);

  if (!booted) return <BootScreen ready={ready} onComplete={finishBoot} />;

  return <Terminal blocks={blocks} onSubmit={submit} ready={ready} error={error} />;
}
