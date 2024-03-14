#ifndef SHELLCOMPONENTS_H
#define SHELLCOMPONENTS_H

#include <stdbool.h>

// From command.c
typedef enum {
    CMD_EXTERNAL,
    CMD_BUILTIN
} CommandType;

typedef struct Command {
    char* command;
    struct Command* next;
    struct Command* previous;
    CommandType type;
    int redirections[2];
    int pipes[2];
    char** options;
    int optionCount;
} Command;

// Function prototypes from command.c
Command* createCommand(char* commandName);
void addOptionToCommand(Command* command, char* option);
void freeCommand(Command* command);
void freeCommandList(Command* head);
void appendCommand(Command** head, Command* newCommand);
void deleteCommand(Command** head, Command* command);
void printCommandList(const Command* head); 
char* strdup(const char* s);

// From builtIns.c
typedef enum {
    CMD_EXIT,
    CMD_STATUS,
    CMD_CD,
    CMD_UNKNOWN
} BuiltInCommand;

// Function prototypes from builtIns.c
void exitShell(char **args);
void statusShell(char **args);
void executeBuiltIns(Command* current);
void updateLastExitStatus (int status);
int cdShell(Command* current);
BuiltInCommand getBuiltInCommand(const char *command);

// Function prototypes from shell.c
void execute(Command* head, char **envp);
void executeCommand(Command* current, char **envp);
void addCommandToOptions(Command* current);

#endif
