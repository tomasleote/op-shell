#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <stdbool.h>

Command *parseInputLine(List *lp, int *parsedSuccessfully);

#endif
