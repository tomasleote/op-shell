export type BlockKind = 'banner' | 'input' | 'output';

/** Uma linha (ou conjunto de linhas) renderizada no terminal. */
export interface TerminalBlock {
  readonly id: number;
  readonly kind: BlockKind;
  readonly text: string;
}
