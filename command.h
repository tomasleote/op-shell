#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    CMD_EXTERNAL, // For external commands
    CMD_BUILTIN   // For built-in commands
} CommandType;

// Command structure
typedef struct Command {
    char* command;         // Command name
    char* commandPath;     // Filesystem path to the executable, if external
    struct Command* next;  // Pointer to the next command in the list
    struct Command* previous; // Pointer to the previous command in the list
    CommandType type;      // Type of command (external or built-in)
    int redirections[2];   // File descriptors for redirections
    int pipes[2];          // File descriptors for pipes
    char** options;        // Options or arguments for the command
    int optionCount;       // Number of options or arguments
} Command;


// Function prototypes
Command* createCommand(char* commandName);
void addOptionToCommand(Command* command, char* option);
void freeCommand(Command* command);
void appendCommand(Command** head, Command* newCommand);
void deleteCommand(Command** head, Command* command);
void printCommands(Command* command);
void printCommand(Command* command);

#endif // COMMAND_H
