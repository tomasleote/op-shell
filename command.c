#include "command.h"
#include <stdlib.h>
#include <string.h>

/**
 * The function createCommand creates a new Command structure.
 * @param commandName the name of the command
*/
Command* createCommand(char* commandName) {
    Command* cmd = (Command*)malloc(sizeof(Command));
    
    if (cmd == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    cmd->command = strdup(commandName); // Duplicate the string to ensure the Command owns its copy
    cmd->options = NULL;
    cmd->optionCount = 0;
    return cmd;
}

/**
 * The function addOptionToCommand adds an option to a Command structure.
 * @param cmd the Command structure to add the option to
 * @param option the option to add
*/
void addOptionToCommand(Command* cmd, char* option) {
    if (cmd == NULL || option == NULL) {
        // Handle null pointer
        return;
    }
    // Resize the options array to hold one more pointer
    char** newOptions = (char**)realloc(cmd->options, sizeof(char*) * (cmd->optionCount + 1));
    if (newOptions == NULL) {
        // Handle memory allocation failure
        return;
    }
    cmd->options = newOptions;
    cmd->options[cmd->optionCount] = strdup(option); // Duplicate the option string
    cmd->optionCount++;
}

/**
 * The function freeCommand frees the memory allocated for a Command structure.
 * @param cmd the Command structure to free
*/
void freeCommand(Command* cmd) {
    if (cmd == NULL) {
        return;
    }
    // Free the command string
    free(cmd->command);
    // Free each option string
    for (int i = 0; i < cmd->optionCount; i++) {
        free(cmd->options[i]);
    }
    // Free the options array itself
    free(cmd->options);
    // Finally, free the Command structure
    free(cmd);
}


//TODO: Implement createCommand, addOptionToCommand, and freeCommand
