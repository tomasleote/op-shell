#ifndef BUILTINS_WEB_H
#define BUILTINS_WEB_H

#include <stddef.h>
#include "command.h"

/* Exit status of the most recent command (defined in shell.c). */
extern int lastExitStatus;

/* Web entry point: runs ONE input line through the real scanner + parser and
   executes it against MEMFS via built-ins. Returns a module-owned buffer that
   stays valid until the next call (the JS side copies it immediately, so it
   must not free it). */
char *execute_line(const char *line);

/* Dispatches a single parsed command to its web built-in. */
void executeWeb(Command *cmd);

/* Shared output buffer for the current execute_line() call. */
void out_append(const char *data, size_t n);
void sh_out(const char *fmt, ...)
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((format(printf, 1, 2)))
#endif
    ;

/* Filesystem built-ins (builtins_fs.c). */
void cmd_ls(Command *cmd);
void cmd_cat(Command *cmd);
void cmd_mkdir(Command *cmd);
void cmd_touch(Command *cmd);
void cmd_rm(Command *cmd);
void cmd_parse(Command *cmd);

#endif /* BUILTINS_WEB_H */
