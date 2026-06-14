import { forwardRef, useState, type KeyboardEvent } from 'react';
import { useHistory } from '../hooks/useHistory';
import { PROMPT } from '../constants/banner';

interface Props {
  onSubmit: (line: string) => void;
  disabled: boolean;
}

/**
 * Linha de input com prompt e cursor de bloco. O <input> real fica por cima,
 * transparente (caret nativo escondido), para o cursor CSS poder piscar e ser
 * desligado em prefers-reduced-motion.
 */
export const TerminalInput = forwardRef<HTMLInputElement, Props>(function TerminalInput(
  { onSubmit, disabled },
  ref,
) {
  const [value, setValue] = useState('');
  const history = useHistory();

  const handleKeyDown = (event: KeyboardEvent<HTMLInputElement>): void => {
    if (event.key === 'Enter') {
      history.add(value);
      onSubmit(value);
      setValue('');
    } else if (event.key === 'ArrowUp') {
      event.preventDefault();
      const prev = history.previous();
      if (prev !== null) setValue(prev);
    } else if (event.key === 'ArrowDown') {
      event.preventDefault();
      const forward = history.next();
      if (forward !== null) setValue(forward);
    }
  };

  return (
    <div className="inputline">
      <span className="inputline__prompt">{PROMPT}</span>
      <span className="inputline__text">{value}</span>
      <span className="inputline__cursor" aria-hidden="true" />
      <input
        ref={ref}
        className="inputline__field"
        type="text"
        value={value}
        onChange={(event) => setValue(event.target.value)}
        onKeyDown={handleKeyDown}
        disabled={disabled}
        spellCheck={false}
        autoComplete="off"
        autoCapitalize="off"
        autoCorrect="off"
        aria-label="Linha de comando"
      />
    </div>
  );
});
