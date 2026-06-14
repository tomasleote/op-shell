export type ExecuteLine = (line: string) => string;

/**
 * Forma minima do modulo WASM consumido pela app (contrato, seccao 6).
 * O modulo real (Seccao 4) e o mock de dev partilham esta interface.
 */
export interface ShellModule {
  cwrap(name: 'execute_line', returnType: 'string', argTypes: ['string']): ExecuteLine;
}

export type ShellModuleFactory = () => Promise<ShellModule>;
