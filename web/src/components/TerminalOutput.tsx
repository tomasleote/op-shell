import type { TerminalBlock } from '../types';

interface Props {
  blocks: readonly TerminalBlock[];
}

export function TerminalOutput({ blocks }: Props) {
  return (
    <>
      {blocks.map((block) => (
        <pre key={block.id} className={`block block--${block.kind}`}>
          {block.text}
        </pre>
      ))}
    </>
  );
}
