#ifndef BUILTINS_H
#define BUILTINS_H

#include "command.h"

typedef enum {
    CMD_EXIT,
    CMD_STATUS,
    CMD_CD,
    CMD_UNKNOWN
} BuiltInCommand;

void exitShell(char **args);
void statusShell(char **args);
void executeBuiltIns(Command* current);
void updateLastExitStatus (int status);
int cdShell(Command* current);
BuiltInCommand getBuiltInCommand(const char *command);

#endif
