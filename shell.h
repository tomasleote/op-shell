#ifndef SHELL_H
#define SHELL_H

#include "command.h" // For Command struct


// Function prototypes
void execute(Command* head, char **envp);
void executeCommand(Command* current, char **envp);
void addCommandToOptions (Command* current);

#endif // SHELL_H

