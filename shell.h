#ifndef SHELL_H
#define SHELL_H

#include "command.h" // For Command struct


// Function prototypes
int exitShell(char **args);
void execute(Command* head, char **envp);
void executeBuiltIns(Command* current);
void executeCommand(Command* current, char **envp);
void addCommandToOptions (Command* current);

#endif // SHELL_H

