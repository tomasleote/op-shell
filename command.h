#ifndef COMMAND_H
#define COMMAND_H

typedef struct {
    char* command;
    char* commandPath;
    int next; 
    int previous;
    int tokenType; 
    int redirections[2]; 
    int pipes[2]; 
    char** options; // Dynamically allocated array of strings
    int optionCount;
} Command;

// Function declarations
Command* createCommand(char* commandName);
void addOptionToCommand(Command* cmd, char* option);
void freeCommand(Command* cmd); // Don't forget to deallocate memory

#endif // COMMAND_H
