#ifndef PARSER_H
#define PARSER_H

#include "command.h" // Include the command struct definition
#include "scanner.h" // Assuming you have a scanner for tokenizing the input

// Function prototypes
bool parseInputLine(List *lp, int *parsedSuccessfully);
bool parseCommand(List *lp, Command** head);
bool parseExecutable(List *lp, Command **head);
bool parseOptions(List *lp, Command** head);
bool parsePipeline(List *lp, Command** head);
bool parseChain(List *lp, Command** head);
bool parseBuiltIn(List *lp, Command **head);
bool parseRedirections(List *lp);
bool parseFileName(List *lp);

#endif // PARSER_H
