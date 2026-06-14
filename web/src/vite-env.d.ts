/// <reference types="vite/client" />

interface ImportMetaEnv {
  readonly VITE_USE_REAL_SHELL?: string;
}

interface ImportMeta {
  readonly env: ImportMetaEnv;
}
