#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a new command
Command* createCommand(char* commandName) {
    Command* newCommand = (Command*)malloc(sizeof(Command));
    if (newCommand == NULL) {
        perror("Unable to allocate memory for new command");
        exit(EXIT_FAILURE);
    }

    newCommand->command = strdup(commandName);
    newCommand->commandPath = NULL; // Initially NULL, can be set later
    newCommand->next = NULL;
    newCommand->previous = NULL;
    newCommand->type = CMD_EXTERNAL; // Default to external command; 
    memset(newCommand->redirections, 0, sizeof(newCommand->redirections));
    memset(newCommand->pipes, 0, sizeof(newCommand->pipes));
    newCommand->options = NULL;
    newCommand->optionCount = 0;

    return newCommand;
}

// Function to add an option to a command
void addOptionToCommand(Command* command, char* option) {
    command->options = realloc(command->options, sizeof(char*) * (command->optionCount + 1));
    if (command->options == NULL) {
        perror("Unable to reallocate memory for options");
        exit(EXIT_FAILURE);
    }
    command->options[command->optionCount] = strdup(option);
    command->optionCount++;
}

// Function to free a command and its associated resources
void freeCommand(Command* command) {
    if (command != NULL) {
        free(command->command);
        free(command->commandPath);
        for (int i = 0; i < command->optionCount; i++) {
            free(command->options[i]);
        }
        free(command->options);
        free(command);
    }
}

// Function to append a command to the end of a list
void appendCommand(Command** head, Command* newCommand) {
    if (*head == NULL) {
        *head = newCommand;
    } else {
        Command* last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newCommand;
        newCommand->previous = last;
    }
}

// Function to delete a command from the list
void deleteCommand(Command** head, Command* command) {
    if (*head == NULL || command == NULL) return;

    if (*head == command) {
        *head = command->next;
    }

    if (command->next != NULL) {
        command->next->previous = command->previous;
    }

    if (command->previous != NULL) {
        command->previous->next = command->next;
    }

    freeCommand(command);
}

// Example function to print a single command (for debugging purposes)
void printCommand(Command* command) {
    printf("Command: %s\n", command->command);
    printf("Type: %d\n", command->type);  

    if (command->commandPath != NULL) {
        printf("Command Path: %s\n", command->commandPath);
    }

    for (int i = 0; i < command->optionCount; i++) {
        printf("Option %d: %s\n", i + 1, command->options[i]);
    }
}

// Example function to print the command list (for debugging purposes)
void printCommands(Command* command) {
    while (command != NULL) {
        printCommand(command);
        command = command->next;
    }
}
